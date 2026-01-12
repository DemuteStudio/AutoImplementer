// Copyright Demute SRL. All Rights Reserved.

#include "GISB_MetasoundPatchBuilder.h"
#include "MetasoundSource.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundBuilderSubsystem.h"
#include "MetasoundBuilderBase.h"
#include "MetasoundDocumentInterface.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "MetasoundEditorSubsystem.h"
#include <cmath>

#include "GisbSoundBankDataAsset.h"
#include "GisbContainerConverter.h"

// Global variables
FString PathGlobal;
UMetaSoundBuilderSubsystem* BuilderGlobal;

void UGISB_MetasoundPatchBuilder::TestBuilder(UGisbSoundBankDataAsset* dataAsset)
{
	if (!dataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Asset is null!"));
		return;
	}

	UGisbContainerBase* RootContainer = dataAsset->RootContainer;
	if (!RootContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("RootContainer is null!"));
		return;
	}

	// Convert runtime container to import container using the converter
	UGisbImportContainerBase* ImportContainer = UGisbContainerConverter::RuntimeToImportContainer(
		RootContainer,
		GetTransientPackage()
	);

	if (!ImportContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to convert runtime container to import container"));
		return;
	}

	BuilderGlobal = GEngine->GetEngineSubsystem<UMetaSoundBuilderSubsystem>();
	PathGlobal = "/Game/A_TESTING/Metasounds";
	UGISB_MetasoundNodeLibrary::SetupNodes();

	UGISB_MetasoundPatchBuilder::BuildChildNode(ImportContainer, "Testing", 0);

}

// ============================================================================
// BuildChildNode - Recursive child patch builder with stereo detection
// ============================================================================

FChildPatchResult UGISB_MetasoundPatchBuilder::BuildChildNode(
	UGisbImportContainerBase* container,
	const FString& ParentName,
	int32 ChildIndex
)
{
	FChildPatchResult Result;

	if (!container)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildChildNode: Container is null"));
		return Result;
	}

	// Store container reference to access cached properties (bIsStereo, bIsLooping)
	Result.Container = container;

	// Dispatch based on container type
	if (UGisbImportContainerSimpleSound* SimpleSound = Cast<UGisbImportContainerSimpleSound>(container))
	{
		// Use wav file name for SimpleSound containers
		FString WavName = SimpleSound->SoundWave ? SimpleSound->SoundWave->GetName() : FString::Printf(TEXT("SimpleSound%d"), ChildIndex);
		FString ChildName = FString::Printf(TEXT("%s_%s"), *ParentName, *WavName);

		Result.Patch = BuildSimpleSoundNode(SimpleSound, ChildName);
	}
	else if (UGisbImportContainerRandom* Random = Cast<UGisbImportContainerRandom>(container))
	{
		// Generate name with container type
		FString ChildName = FString::Printf(TEXT("%s_Random%d"), *ParentName, ChildIndex);

		// Build Random patch (will recursively build children)
		Result.Patch = BuildRandomNode(Random, ChildName);
	}
	else if (UGisbImportContainerSwitch* Switch = Cast<UGisbImportContainerSwitch>(container))
	{
		// Generate name with container type
		FString ChildName = FString::Printf(TEXT("%s_Switch%d"), *ParentName, ChildIndex);

		Result.Patch = BuildSwitchNode(Switch, ChildName);

		// Track the switch parameter this child needs
		FName SwitchParameterName = Switch->ParameterID.IsNone() ?
			FName("Switch Parameter") : Switch->ParameterID;
		Result.RequiredInputs.Add(FGisbPinInfo(SwitchParameterName, FName("String")));
	}
	else if (UGisbImportContainerBlend* Blend = Cast<UGisbImportContainerBlend>(container))
	{
		// Generate name with container type
		FString ChildName = FString::Printf(TEXT("%s_Blend%d"), *ParentName, ChildIndex);

		Result.Patch = BuildBlendNode(Blend, ChildName);
	}
	else if (UGisbImportContainerTrigger* Trigger = Cast<UGisbImportContainerTrigger>(container))
	{
		// Generate name with container type
		FString ChildName = FString::Printf(TEXT("%s_Trigger%d"), *ParentName, ChildIndex);

		Result.Patch = BuildTriggerNode(Trigger, ChildName);
	}
	else
	{
		FString ChildName = FString::Printf(TEXT("%s_Unknown%d"), *ParentName, ChildIndex);
		UE_LOG(LogTemp, Error, TEXT("BuildChildNode: Unknown container type for %s"), *ChildName);
	}

	if (!Result.Patch)
	{
		// Note: ChildName is scoped to each case, so we recreate it for error log
		FString ChildName = FString::Printf(TEXT("%s_Failed%d"), *ParentName, ChildIndex);
		UE_LOG(LogTemp, Error, TEXT("BuildChildNode: Failed to build patch for %s"), *ChildName);
	}

	return Result;
}

TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundPatchBuilder::BuildSimpleSoundNode(UGisbImportContainerSimpleSound* simpleSound, const FString& Name)
{
	if (!simpleSound)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundNode: Container is null"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Determine if stereo
	bool isStereo = simpleSound->SoundWave && simpleSound->SoundWave->NumChannels > 1;

	// Create patch builder
	UMetaSoundPatchBuilder* builder = BuilderGlobal->CreatePatchBuilder(
		FName(Name), result);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundNode: Failed to create patch builder for %s"), *Name);
		return nullptr;
	}

	// Create graph input for OnPlay trigger
	FMetaSoundBuilderNodeOutputHandle PlayTrigger = builder->AddGraphInputNode(
		FName("Play"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
	FMetaSoundNodeHandle triggerInputNode(PlayTrigger.NodeID);

	// Create graph output for OnFinished trigger (only if not looping)
	FMetaSoundBuilderNodeInputHandle OnFinished;
	FMetaSoundNodeHandle onFinishedNode;
	if (!simpleSound->bIsLooping)
	{
		OnFinished = builder->AddGraphOutputNode(
			FName("On Finished"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
		onFinishedNode = FMetaSoundNodeHandle(OnFinished.NodeID);
	}

	// Create graph output for audio left/mono
	FMetaSoundBuilderNodeInputHandle AudioLeft = builder->AddGraphOutputNode(
		FName(isStereo ? "Audio Left" : "Audio Mono"), FName("Audio"), FMetasoundFrontendLiteral(), result);
	FMetaSoundNodeHandle audioLeftNode(AudioLeft.NodeID);

	// Create graph output for audio right (if stereo)
	FMetaSoundBuilderNodeInputHandle AudioRight;
	FMetaSoundNodeHandle audioRightNode;
	if (isStereo)
	{
		AudioRight = builder->AddGraphOutputNode(
			FName("Audio Right"), FName("Audio"), FMetasoundFrontendLiteral(), result);
		audioRightNode = FMetaSoundNodeHandle(AudioRight.NodeID);
	}

	// Create layout manager
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Register graph I/O nodes with layout manager
	Layout.RegisterGraphInputNode(triggerInputNode, FName("Play"));
	if (!simpleSound->bIsLooping)
	{
		Layout.RegisterGraphOutputNode(onFinishedNode, FName("On Finished"));
	}
	Layout.RegisterGraphOutputNode(audioLeftNode, FName(isStereo ? "Audio Left" : "Audio Mono"));
	if (isStereo)
	{
		Layout.RegisterGraphOutputNode(audioRightNode, FName("Audio Right"));
	}

	// Call core method (does all the work!)
	BuildSimpleSoundCore(
		builder, simpleSound, 
		PlayTrigger, OnFinished,
		AudioLeft, isStereo ? &AudioRight : nullptr,
		&Layout
	);

	// Apply layout and build
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> patch = EditorSubsystem->BuildToAsset(builder, "ISX - Demute", Name, PathGlobal, result);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSoundNode: BuildToAsset FAILED for: %s"), *Name);
		return patch;
	}

	UE_LOG(LogTemp, Log, TEXT("BuildSimpleSoundNode: Successfully built MetaSound for: %s"), *Name);
	return patch;
}


// ============================================================================
// BuildRandomNode - Build MetaSound patch for Random container
// ============================================================================

TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundPatchBuilder::BuildRandomNode(
	UGisbImportContainerRandom* randomContainer,
	const FString& Name
)
{
	if (!randomContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomNode: Container is null"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;
	bool isStereo = randomContainer->bIsStereo;

	// Create patch builder
	UMetaSoundPatchBuilder* builder = BuilderGlobal->CreatePatchBuilder(FName(Name), result);
	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomNode: Failed to create patch builder for %s"), *Name);
		return nullptr;
	}

	// Create graph input for OnPlay trigger
	FMetaSoundBuilderNodeOutputHandle PlayTrigger = builder->AddGraphInputNode(
		FName("Play"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
	FMetaSoundNodeHandle triggerInputNode(PlayTrigger.NodeID);

	// Create graph output for OnFinished trigger (only if not looping)
	FMetaSoundBuilderNodeInputHandle OnFinished;
	FMetaSoundNodeHandle onFinishedNode;
	if (!randomContainer->bIsLooping)
	{
		OnFinished = builder->AddGraphOutputNode(
			FName("On Finished"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
		onFinishedNode = FMetaSoundNodeHandle(OnFinished.NodeID);
	}

	// Create graph output for audio left/mono
	FMetaSoundBuilderNodeInputHandle AudioLeft = builder->AddGraphOutputNode(
		FName(isStereo ? "Audio Left" : "Audio Mono"), FName("Audio"), FMetasoundFrontendLiteral(), result);
	FMetaSoundNodeHandle audioLeftNode(AudioLeft.NodeID);

	// Create graph output for audio right (if stereo)
	FMetaSoundBuilderNodeInputHandle AudioRight;
	FMetaSoundNodeHandle audioRightNode;
	if (isStereo)
	{
		AudioRight = builder->AddGraphOutputNode(
			FName("Audio Right"), FName("Audio"), FMetasoundFrontendLiteral(), result);
		audioRightNode = FMetaSoundNodeHandle(AudioRight.NodeID);
	}

	// Create layout manager
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Register graph I/O nodes with layout manager
	Layout.RegisterGraphInputNode(triggerInputNode, FName("Play"));
	if (!randomContainer->bIsLooping)
	{
		Layout.RegisterGraphOutputNode(onFinishedNode, FName("On Finished"));
	}
	Layout.RegisterGraphOutputNode(audioLeftNode, FName(isStereo ? "Audio Left" : "Audio Mono"));
	if (isStereo)
	{
		Layout.RegisterGraphOutputNode(audioRightNode, FName("Audio Right"));
	}

	// Call core method (does all the work!)
	BuildRandomCore(
		builder, randomContainer, Name,
		PlayTrigger, OnFinished,
		AudioLeft, isStereo ? &AudioRight : nullptr,
		&Layout
	);

	// Apply layout and build
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> patch = EditorSubsystem->BuildToAsset(builder, "ISX - Demute", Name, PathGlobal, result);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomNode: BuildToAsset FAILED for: %s"), *Name);
		return patch;
	}

	UE_LOG(LogTemp, Log, TEXT("BuildRandomNode: Successfully built MetaSound for: %s"), *Name);
	return patch;
}

// ============================================================================
// BuildBlendNode - Build MetaSound patch for Blend container
// ============================================================================

TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundPatchBuilder::BuildBlendNode(
	UGisbImportContainerBlend* blendContainer,
	const FString& Name
)
{
	if (!blendContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendNode: Container is null"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Determine stereo format by checking children recursively
	bool isStereo = blendContainer->bIsStereo;

	// Create patch builder with graph inputs/outputs
	FMetaSoundBuilderNodeOutputHandle PlayTrigger;
	FMetaSoundBuilderNodeInputHandle OnFinished;
	FMetaSoundBuilderNodeInputHandle AudioLeft;
	FMetaSoundBuilderNodeInputHandle AudioRight;

	UMetaSoundPatchBuilder* builder = BuilderGlobal->CreatePatchBuilder(
		FName(Name), result);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendNode: Failed to create patch builder for %s"), *Name);
		return nullptr;
	}

	// Create graph input for OnPlay trigger
	PlayTrigger = builder->AddGraphInputNode(
		FName("Play"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
	FMetaSoundNodeHandle triggerInputNode(PlayTrigger.NodeID);

	// Create graph output for OnFinished trigger (only if not looping)
	FMetaSoundNodeHandle onFinishedNode;
	if (!blendContainer->bIsLooping)
	{
		OnFinished = builder->AddGraphOutputNode(
			FName("On Finished"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
		onFinishedNode = FMetaSoundNodeHandle(OnFinished.NodeID);
	}

	// Create graph output for audio left/mono
	AudioLeft = builder->AddGraphOutputNode(
		FName(isStereo ? "Audio Left" : "Audio Mono"), FName("Audio"), FMetasoundFrontendLiteral(), result);
	FMetaSoundNodeHandle audioLeftNode(AudioLeft.NodeID);

	// Create graph output for audio right (if stereo)
	FMetaSoundNodeHandle audioRightNode;
	if (isStereo)
	{
		AudioRight = builder->AddGraphOutputNode(
			FName("Audio Right"), FName("Audio"), FMetasoundFrontendLiteral(), result);
		audioRightNode = FMetaSoundNodeHandle(AudioRight.NodeID);
	}

	// Create layout manager
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Register graph I/O nodes with layout manager
	Layout.RegisterGraphInputNode(triggerInputNode, FName("Play"));
	if (!blendContainer->bIsLooping)
	{
		Layout.RegisterGraphOutputNode(onFinishedNode, FName("On Finished"));
	}
	Layout.RegisterGraphOutputNode(audioLeftNode, FName(isStereo ? "Audio Left" : "Audio Mono"));
	if (isStereo)
	{
		Layout.RegisterGraphOutputNode(audioRightNode, FName("Audio Right"));
	}

	// Call core method (does all the work!)
	BuildBlendCore(
		builder, blendContainer, Name,
		PlayTrigger, OnFinished,
		AudioLeft, isStereo ? &AudioRight : nullptr,
		&Layout
	);

	// Apply layout and build
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> patch = EditorSubsystem->BuildToAsset(builder, "ISX - Demute", Name, PathGlobal, result);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendNode: BuildToAsset FAILED for: %s"), *Name);
		return patch;
	}

	UE_LOG(LogTemp, Log, TEXT("BuildBlendNode: Successfully built MetaSound for: %s"), *Name);
	return patch;
}

// ============================================================================
// BuildSwitchNode - Build MetaSound patch for Switch container
// ============================================================================

TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundPatchBuilder::BuildSwitchNode(
	UGisbImportContainerSwitch* switchContainer,
	const FString& Name
)
{
	if (!switchContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchNode: Container is null"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Determine stereo format by checking children recursively
	bool isStereo = switchContainer->bIsStereo;

	// Create patch builder
	UMetaSoundPatchBuilder* builder = BuilderGlobal->CreatePatchBuilder(
		FName(Name), result);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchNode: Failed to create patch builder for %s"), *Name);
		return nullptr;
	}

	// Create graph input for OnPlay trigger
	FMetaSoundBuilderNodeOutputHandle PlayTrigger = builder->AddGraphInputNode(
		FName("Play"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
	FMetaSoundNodeHandle triggerInputNode(PlayTrigger.NodeID);

	// Create graph output for OnFinished trigger (only if not looping)
	FMetaSoundBuilderNodeInputHandle OnFinished;
	FMetaSoundNodeHandle onFinishedNode;
	if (!switchContainer->bIsLooping)
	{
		OnFinished = builder->AddGraphOutputNode(
			FName("On Finished"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
		onFinishedNode = FMetaSoundNodeHandle(OnFinished.NodeID);
	}

	// Create graph output for audio left/mono
	FMetaSoundBuilderNodeInputHandle AudioLeft = builder->AddGraphOutputNode(
		FName(isStereo ? "Audio Left" : "Audio Mono"), FName("Audio"), FMetasoundFrontendLiteral(), result);
	FMetaSoundNodeHandle audioLeftNode(AudioLeft.NodeID);

	// Create graphre output for audio right (if stereo)
	FMetaSoundBuilderNodeInputHandle AudioRight;
	FMetaSoundNodeHandle audioRightNode;
	if (isStereo)
	{
		AudioRight = builder->AddGraphOutputNode(
			FName("Audio Right"), FName("Audio"), FMetasoundFrontendLiteral(), result);
		audioRightNode = FMetaSoundNodeHandle(AudioRight.NodeID);
	}

	// Extract parameter name from container (use custom name from JSON, fallback to "Switch Parameter")
	FName SwitchParameterName = switchContainer->ParameterID.IsNone() ?
		FName("Switch Parameter") : switchContainer->ParameterID;

	// Add Switch Parameter input (String type) - unique to switch containers
	FMetaSoundBuilderNodeOutputHandle ParameterInput = builder->AddGraphInputNode(
		SwitchParameterName,  // Use container's parameter name instead of hardcoded
		FName("String"),
		FMetasoundFrontendLiteral(),
		result
	);
	FMetaSoundNodeHandle parameterInputNode(ParameterInput.NodeID);

	// Create layout manager
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Register graph I/O nodes with layout manager
	Layout.RegisterGraphInputNode(triggerInputNode, FName("Play"));
	Layout.RegisterGraphInputNode(parameterInputNode, SwitchParameterName);
	if (!switchContainer->bIsLooping)
	{
		Layout.RegisterGraphOutputNode(onFinishedNode, FName("On Finished"));
	}
	Layout.RegisterGraphOutputNode(audioLeftNode, FName(isStereo ? "Audio Left" : "Audio Mono"));
	if (isStereo)
	{
		Layout.RegisterGraphOutputNode(audioRightNode, FName("Audio Right"));
	}

	// Call core method (does all the work!)
	BuildSwitchCore(
		builder, switchContainer, Name,
		ParameterInput, PlayTrigger, OnFinished,
		AudioLeft, isStereo ? &AudioRight : nullptr,
		&Layout
	);

	// Apply layout and build
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> patch = EditorSubsystem->BuildToAsset(builder, "ISX - Demute", Name, PathGlobal, result);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchNode: BuildToAsset FAILED for: %s"), *Name);
		return patch;
	}

	UE_LOG(LogTemp, Log, TEXT("BuildSwitchNode: Successfully built MetaSound for: %s"), *Name);
	return patch;
}

// ============================================================================
// BuildTriggerNode - Build MetaSound patch for Trigger container
// ============================================================================

TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundPatchBuilder::BuildTriggerNode(
	UGisbImportContainerTrigger* triggerContainer,
	const FString& Name
)
{
	if (!triggerContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTriggerNode: Container is null"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Determine if stereo
	bool isStereo = triggerContainer->TriggeredSoundImport->bIsStereo;

	// Create patch builder
	UMetaSoundPatchBuilder* builder = BuilderGlobal->CreatePatchBuilder(
		FName(Name), result);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTriggerNode: Failed to create patch builder for %s"), *Name);
		return nullptr;
	}

	// Create graph input for OnPlay trigger
	FMetaSoundBuilderNodeOutputHandle PlayTrigger = builder->AddGraphInputNode(
		FName("Play"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
	FMetaSoundNodeHandle triggerInputNode(PlayTrigger.NodeID);

	// Create graph output for OnFinished trigger (only if not looping)
	FMetaSoundBuilderNodeInputHandle OnFinished;
	FMetaSoundNodeHandle onFinishedNode;
	if (!triggerContainer->bIsLooping)
	{
		OnFinished = builder->AddGraphOutputNode(
			FName("On Finished"), FName("Trigger"), FMetasoundFrontendLiteral(), result);
		onFinishedNode = FMetaSoundNodeHandle(OnFinished.NodeID);
	}

	// Create graph output for audio left/mono
	FMetaSoundBuilderNodeInputHandle AudioLeft = builder->AddGraphOutputNode(
		FName(isStereo ? "Audio Left" : "Audio Mono"), FName("Audio"), FMetasoundFrontendLiteral(), result);
	FMetaSoundNodeHandle audioLeftNode(AudioLeft.NodeID);

	// Create graph output for audio right (if stereo)
	FMetaSoundBuilderNodeInputHandle AudioRight;
	FMetaSoundNodeHandle audioRightNode;
	if (isStereo)
	{
		AudioRight = builder->AddGraphOutputNode(
			FName("Audio Right"), FName("Audio"), FMetasoundFrontendLiteral(), result);
		audioRightNode = FMetaSoundNodeHandle(AudioRight.NodeID);
	}

	// Create layout manager
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Register graph I/O nodes with layout manager
	Layout.RegisterGraphInputNode(triggerInputNode, FName("Play"));
	if (!triggerContainer->bIsLooping)
	{
		Layout.RegisterGraphOutputNode(onFinishedNode, FName("On Finished"));
	}
	Layout.RegisterGraphOutputNode(audioLeftNode, FName(isStereo ? "Audio Left" : "Audio Mono"));
	if (isStereo)
	{
		Layout.RegisterGraphOutputNode(audioRightNode, FName("Audio Right"));
	}

	// Call core method (does all the work!)
	BuildTriggerCore(
		builder, triggerContainer, Name,
		PlayTrigger, OnFinished,
		AudioLeft, isStereo ? &AudioRight : nullptr,
		&Layout
	);

	// Apply layout and build
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> patch = EditorSubsystem->BuildToAsset(builder, "ISX - Demute", Name, PathGlobal, result);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTriggerNode: BuildToAsset FAILED for: %s"), *Name);
		return patch;
	}

	UE_LOG(LogTemp, Log, TEXT("BuildTriggerNode: Successfully built MetaSound for: %s"), *Name);
	return patch;
}

// ============================================================================
// Note: Helper methods (setupProbability, setupAttributes, ConnectProbabilityNode,
// ConnectVolume, ConnectPitch, ConnectLowpass, isStereo) have been moved to the
// base class UGISB_MetasoundBuilderCore and are now inherited.
// ============================================================================
