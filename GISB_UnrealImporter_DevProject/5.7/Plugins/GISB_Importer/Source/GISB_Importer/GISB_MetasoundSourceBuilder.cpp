// Fill out your copyright notice in the Description page of Project Settings.


#include "GISB_MetasoundSourceBuilder.h"
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
#include "GISB_MetasoundPatchBuilder.h"
#include "GisbMetasoundLayoutManager.h"

// Global variables for source builder
FString PathGlobal_Source;
UMetaSoundBuilderSubsystem* BuilderGlobal_Source;

// External references to PatchBuilder globals (defined in GISB_MetasoundPatchBuilder.cpp)
extern FString PathGlobal;
extern UMetaSoundBuilderSubsystem* BuilderGlobal;

UMetaSoundSource* UGISB_MetasoundSourceBuilder::TestBuilder(UGisbSoundBankDataAsset* dataAsset)
{
	if (!dataAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("TestBuilder: DataAsset is null!"));
		return nullptr;
	}

	UGisbContainerBase* RootContainer = dataAsset->RootContainer;
	if (!RootContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("TestBuilder: RootContainer is null!"));
		return nullptr;
	}

	// Convert runtime container to import container using the converter
	UGisbImportContainerBase* ImportContainer = UGisbContainerConverter::RuntimeToImportContainer(
		RootContainer,
		GetTransientPackage()
	);

	if (!ImportContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("TestBuilder: Failed to convert runtime container to import container"));
		return nullptr;
	}

	// Build MetaSound source (CreateMetasoundFromGISB handles all initialization)
	UMetaSoundSource* source = CreateMetasoundFromGISB(
		ImportContainer,
		TEXT("TestSource"),
		TEXT("/Game/A_TESTING/Metasounds")
	);

	if (source)
	{
		UE_LOG(LogTemp, Log, TEXT("TestBuilder: Successfully created MetaSound Source!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TestBuilder: Failed to create MetaSound Source"));
	}

	return source;
}

UMetaSoundSource* UGISB_MetasoundSourceBuilder::CreateMetasoundFromGISB(UGisbImportContainerBase* gisb, const FString& Name, const FString& path)
{
	if (!gisb)
	{
		UE_LOG(LogTemp, Error, TEXT("CreateMetasoundFromGISB: Container is null"));
		return nullptr;
	}

	// Initialize global subsystems
	BuilderGlobal_Source = GEngine->GetEngineSubsystem<UMetaSoundBuilderSubsystem>();
	PathGlobal_Source = path;

	// Initialize PatchBuilder globals for child patches
	// Child patches will be created in a "Patches" subfolder
	BuilderGlobal = BuilderGlobal_Source;  // Use same builder subsystem
	PathGlobal = path + "/Patches";        // Child patches go in subfolder

	UGISB_MetasoundNodeLibrary::SetupNodes();

	// Get cached stereo and looping properties
	bool shouldLoop = gisb->bIsLooping;
	bool shouldStereo = gisb->bIsStereo;

	// Dispatch to appropriate Build method based on container type
	UMetaSoundSource* source;

	if (UGisbImportContainerSimpleSound* SimpleSound = Cast<UGisbImportContainerSimpleSound>(gisb))
	{
		source = BuildSimpleSource(SimpleSound, Name, shouldLoop);
	}
	else if (UGisbImportContainerRandom* Random = Cast<UGisbImportContainerRandom>(gisb))
	{
		source = BuildRandomSource(Random, Name, shouldLoop);
	}
	else if (UGisbImportContainerBlend* Blend = Cast<UGisbImportContainerBlend>(gisb))
	{
		source = BuildBlendSource(Blend, Name, shouldLoop);
	}
	else if (UGisbImportContainerSwitch* Switch = Cast<UGisbImportContainerSwitch>(gisb))
	{
		source = BuildSwitchSource(Switch, Name, shouldLoop);
	}
	else if (UGisbImportContainerTrigger* Trigger = Cast<UGisbImportContainerTrigger>(gisb))
	{
		source = BuildTriggerSource(Trigger, Name, shouldLoop);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unknown container type"));
		return nullptr;
	}

	// Apply attenuation if present
	if (source && gisb->Attenuation.overrideParent)
	{
		source->AttenuationSettings = gisb->Attenuation.soundAttenuation;
	}

	return source;
}

void UGISB_MetasoundSourceBuilder::DetectLoopAndMono(UGisbImportContainerBase* gisb, bool& canLoop, bool& shouldStereo)
{
	if (canLoop && shouldStereo) return;

	UGisbImportContainerSimpleSound* simplesound = Cast<UGisbImportContainerSimpleSound>(gisb);
	if (simplesound != nullptr)
	{
		if(simplesound->loop) canLoop = true;
		if(simplesound->SoundWave && simplesound->SoundWave->NumChannels > 1) shouldStereo = true;
	}
	
	UGisbImportContainerBlend* blend = Cast<UGisbImportContainerBlend>(gisb);
	if (blend != nullptr)
	{
		for (UGisbImportContainerBase* child : blend->SoundArray)
		{
			DetectLoopAndMono(child, canLoop, shouldStereo);
		}
	}

	UGisbImportContainerRandom* random = Cast<UGisbImportContainerRandom>(gisb);
	if (random != nullptr)
	{
		for (UGisbImportContainerBase* child : random->SoundArray)
		{
			DetectLoopAndMono(child, canLoop, shouldStereo);
		}
	}

	UGisbImportContainerSwitch* swtch = Cast<UGisbImportContainerSwitch>(gisb);
	if (swtch != nullptr)
	{
		for (auto& child : swtch->SoundDictionary)
		{
			DetectLoopAndMono(child.Value, canLoop, shouldStereo);
		}
	}
}

// ============================================================================
// BuildSimpleSource - Build MetaSound source for SimpleSound container
// ============================================================================

UMetaSoundSource* UGISB_MetasoundSourceBuilder::BuildSimpleSource(
	UGisbImportContainerSimpleSound* simpleSound,
	const FString& Name,
	bool canLoop
)
{
	if (!simpleSound)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSource: Container is null"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Determine stereo format
	bool bisStereo = simpleSound->SoundWave && simpleSound->SoundWave->NumChannels > 1;
	EMetaSoundOutputAudioFormat format = bisStereo ?
		EMetaSoundOutputAudioFormat::Stereo : EMetaSoundOutputAudioFormat::Mono;

	// ========================================================================
	// PHASE 1: Create Source Builder (auto-creates handles)
	// ========================================================================

	FMetaSoundBuilderNodeOutputHandle OnPlayNode;
	FMetaSoundBuilderNodeInputHandle OnFinishedNode;
	TArray<FMetaSoundBuilderNodeInputHandle> outAudioHandles;

	UMetaSoundSourceBuilder* builder = BuilderGlobal_Source->CreateSourceBuilder(
		FName(Name),
		OnPlayNode,
		OnFinishedNode,
		outAudioHandles,
		result,
		format,
		!canLoop  // OneShot if can't loop
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSource: Failed to create source builder for %s"), *Name);
		return nullptr;
	}

	builder->SetAuthor("ISX - Demute");

	// Create layout manager for node positioning
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Extract node handles from builder handles
	FMetaSoundNodeHandle onPlayNodeHandle(OnPlayNode.NodeID);
	FMetaSoundNodeHandle onFinishedNodeHandle(OnFinishedNode.NodeID);

	// Register graph I/O nodes with layout
	Layout.RegisterGraphInputNode(onPlayNodeHandle, FName("Play"));
	if (OnFinishedNode.IsSet())
	{
		Layout.RegisterGraphOutputNode(onFinishedNodeHandle, FName("On Finished"));
	}

	// Register audio output(s)
	FMetaSoundNodeHandle audioLeftNodeHandle(outAudioHandles[0].NodeID);
	Layout.RegisterGraphOutputNode(audioLeftNodeHandle,
		FName(bisStereo ? "Audio Left" : "Audio Mono"));

	if (bisStereo && outAudioHandles.Num() > 1)
	{
		FMetaSoundNodeHandle audioRightNodeHandle(outAudioHandles[1].NodeID);
		Layout.RegisterGraphOutputNode(audioRightNodeHandle, FName("Audio Right"));
	}

	// ========================================================================
	// PHASE 2: Build Core Logic (from base class - builder-agnostic!)
	// BuildCore connects directly to graph outputs (Pattern A)
	// ========================================================================

	if (outAudioHandles.Num() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSource: No audio output handles created"));
		return nullptr;
	}

	BuildSimpleSoundCore(
		builder,              // Implicitly casts to UMetaSoundBuilderBase*
		simpleSound,
		OnPlayNode,           // Trigger input (auto-created by source builder)
		OnFinishedNode,       // On finished input (auto-created)
		outAudioHandles[0],   // Graph output left/mono (BuildCore connects automatically)
		(bisStereo && outAudioHandles.Num() > 1) ? &outAudioHandles[1] : nullptr,  // Graph output right (nullable)
		&Layout               // Pass layout manager instead of nullptr
	);

	// ========================================================================
	// PHASE 3: Apply Layout and Build to Asset
	// ========================================================================

	// Compute and apply layout before building asset
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> document = EditorSubsystem->BuildToAsset(
		builder,
		"ISX - Demute",
		Name,
		PathGlobal_Source,
		result
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSimpleSource: BuildToAsset FAILED for: %s"), *Name);
		return nullptr;
	}

	UMetaSoundSource* source = Cast<UMetaSoundSource>(document.GetObject());
	UE_LOG(LogTemp, Log, TEXT("BuildSimpleSource: Successfully built MetaSound Source for: %s"), *Name);

	return source;
}

// ============================================================================
// BuildRandomSource - Build MetaSound source for Random container
// ============================================================================

UMetaSoundSource* UGISB_MetasoundSourceBuilder::BuildRandomSource(
	UGisbImportContainerRandom* randomContainer,
	const FString& Name,
	bool canLoop
)
{
	if (!randomContainer || randomContainer->SoundArray.Num() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomSource: Invalid container"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Determine stereo format by checking children
	bool bisStereo = randomContainer->bIsStereo;
	EMetaSoundOutputAudioFormat format = bisStereo ?
		EMetaSoundOutputAudioFormat::Stereo : EMetaSoundOutputAudioFormat::Mono;

	// ========================================================================
	// PHASE 1: Create Source Builder + Build Child Patches
	// ========================================================================

	FMetaSoundBuilderNodeOutputHandle OnPlayNode;
	FMetaSoundBuilderNodeInputHandle OnFinishedNode;
	TArray<FMetaSoundBuilderNodeInputHandle> outAudioHandles;

	UMetaSoundSourceBuilder* builder = BuilderGlobal_Source->CreateSourceBuilder(
		FName(Name), OnPlayNode, OnFinishedNode, outAudioHandles, result, format, !canLoop
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomSource: Failed to create source builder"));
		return nullptr;
	}

	builder->SetAuthor("ISX - Demute");

	// Create layout manager for node positioning
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Extract node handles from builder handles
	FMetaSoundNodeHandle onPlayNodeHandle(OnPlayNode.NodeID);
	FMetaSoundNodeHandle onFinishedNodeHandle(OnFinishedNode.NodeID);

	// Register graph I/O nodes with layout
	Layout.RegisterGraphInputNode(onPlayNodeHandle, FName("Play"));
	if (OnFinishedNode.IsSet())
	{
		Layout.RegisterGraphOutputNode(onFinishedNodeHandle, FName("On Finished"));
	}

	// Register audio output(s)
	FMetaSoundNodeHandle audioLeftNodeHandle(outAudioHandles[0].NodeID);
	Layout.RegisterGraphOutputNode(audioLeftNodeHandle,
		FName(bisStereo ? "Audio Left" : "Audio Mono"));

	if (bisStereo && outAudioHandles.Num() > 1)
	{
		FMetaSoundNodeHandle audioRightNodeHandle(outAudioHandles[1].NodeID);
		Layout.RegisterGraphOutputNode(audioRightNodeHandle, FName("Audio Right"));
	}

	// ========================================================================
	// PHASE 2: Build Core Logic (from base class)
	// BuildCore builds children and connects to graph outputs
	// ========================================================================

	BuildRandomCore(
		builder,                                      // Source builder
		randomContainer,                              // Container object
		Name,                                         // Base name
		OnPlayNode,                                   // Trigger (auto-created)
		OnFinishedNode,                               // On finished (auto-created)
		outAudioHandles[0],                           // Graph output left/mono (BuildCore connects automatically)
		(bisStereo && outAudioHandles.Num() > 1) ? &outAudioHandles[1] : nullptr,  // Graph output right (nullable)
		&Layout                                       // Pass layout manager instead of nullptr
	);

	// ========================================================================
	// PHASE 3: Apply Layout and Build to Asset
	// ========================================================================

	// Compute and apply layout before building asset
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> document = EditorSubsystem->BuildToAsset(
		builder, "ISX - Demute", Name, PathGlobal_Source, result
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomSource: BuildToAsset FAILED"));
		return nullptr;
	}

	UMetaSoundSource* source = Cast<UMetaSoundSource>(document.GetObject());
	UE_LOG(LogTemp, Log, TEXT("BuildRandomSource: Successfully built for: %s"), *Name);

	return source;
}

// ============================================================================
// BuildBlendSource - Build MetaSound source for Blend container
// ============================================================================

UMetaSoundSource* UGISB_MetasoundSourceBuilder::BuildBlendSource(
	UGisbImportContainerBlend* blendContainer,
	const FString& Name,
	bool canLoop
)
{
	if (!blendContainer || blendContainer->SoundArray.Num() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendSource: Invalid container"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Determine stereo format
	bool bisStereo = blendContainer->bIsStereo;
	EMetaSoundOutputAudioFormat format = bisStereo ?
		EMetaSoundOutputAudioFormat::Stereo : EMetaSoundOutputAudioFormat::Mono;

	// ========================================================================
	// PHASE 1: Create Source Builder + Build Child Patches
	// ========================================================================

	FMetaSoundBuilderNodeOutputHandle OnPlayNode;
	FMetaSoundBuilderNodeInputHandle OnFinishedNode;
	TArray<FMetaSoundBuilderNodeInputHandle> outAudioHandles;

	UMetaSoundSourceBuilder* builder = BuilderGlobal_Source->CreateSourceBuilder(
		FName(Name), OnPlayNode, OnFinishedNode, outAudioHandles, result, format, !canLoop
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendSource: Failed to create source builder"));
		return nullptr;
	}

	builder->SetAuthor("ISX - Demute");

	// Create layout manager for node positioning
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Extract node handles from builder handles
	FMetaSoundNodeHandle onPlayNodeHandle(OnPlayNode.NodeID);
	FMetaSoundNodeHandle onFinishedNodeHandle(OnFinishedNode.NodeID);

	// Register graph I/O nodes with layout
	Layout.RegisterGraphInputNode(onPlayNodeHandle, FName("Play"));
	if (OnFinishedNode.IsSet())
	{
		Layout.RegisterGraphOutputNode(onFinishedNodeHandle, FName("On Finished"));
	}

	// Register audio output(s)
	FMetaSoundNodeHandle audioLeftNodeHandle(outAudioHandles[0].NodeID);
	Layout.RegisterGraphOutputNode(audioLeftNodeHandle,
		FName(bisStereo ? "Audio Left" : "Audio Mono"));

	if (bisStereo && outAudioHandles.Num() > 1)
	{
		FMetaSoundNodeHandle audioRightNodeHandle(outAudioHandles[1].NodeID);
		Layout.RegisterGraphOutputNode(audioRightNodeHandle, FName("Audio Right"));
	}

	// ========================================================================
	// PHASE 2: Build Core Logic (from base class)
	// BuildCore connects directly to graph outputs (Pattern A)
	// ========================================================================

	BuildBlendCore(
		builder,
		blendContainer,
		Name,
		OnPlayNode,
		OnFinishedNode,
		outAudioHandles[0],
		(bisStereo && outAudioHandles.Num() > 1) ? &outAudioHandles[1] : nullptr,
		&Layout
	);

	// ========================================================================
	// PHASE 3: Apply Layout and Build to Asset
	// ========================================================================

	// Compute and apply layout before building asset
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> document = EditorSubsystem->BuildToAsset(
		builder, "ISX - Demute", Name, PathGlobal_Source, result
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildBlendSource: BuildToAsset FAILED"));
		return nullptr;
	}

	UMetaSoundSource* source = Cast<UMetaSoundSource>(document.GetObject());
	UE_LOG(LogTemp, Log, TEXT("BuildBlendSource: Successfully built for: %s"), *Name);

	return source;
}

// ============================================================================
// BuildSwitchSource - Build MetaSound source for Switch container
// ============================================================================

UMetaSoundSource* UGISB_MetasoundSourceBuilder::BuildSwitchSource(
	UGisbImportContainerSwitch* switchContainer,
	const FString& Name,
	bool canLoop
)
{
	if (!switchContainer || switchContainer->SoundDictionary.Num() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchSource: Invalid container"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Determine stereo format
	bool bisStereo = switchContainer->bIsStereo;
	EMetaSoundOutputAudioFormat format = bisStereo ?
		EMetaSoundOutputAudioFormat::Stereo : EMetaSoundOutputAudioFormat::Mono;

	// ========================================================================
	// PHASE 1: Create Source Builder + Build Child Patches
	// ========================================================================

	FMetaSoundBuilderNodeOutputHandle OnPlayNode;
	FMetaSoundBuilderNodeInputHandle OnFinishedNode;
	TArray<FMetaSoundBuilderNodeInputHandle> outAudioHandles;

	UMetaSoundSourceBuilder* builder = BuilderGlobal_Source->CreateSourceBuilder(
		FName(Name), OnPlayNode, OnFinishedNode, outAudioHandles, result, format, !canLoop
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchSource: Failed to create source builder"));
		return nullptr;
	}

	builder->SetAuthor("ISX - Demute");

	// Create layout manager for node positioning
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Extract node handles from builder handles
	FMetaSoundNodeHandle onPlayNodeHandle(OnPlayNode.NodeID);
	FMetaSoundNodeHandle onFinishedNodeHandle(OnFinishedNode.NodeID);

	// Register graph I/O nodes with layout
	Layout.RegisterGraphInputNode(onPlayNodeHandle, FName("Play"));
	if (OnFinishedNode.IsSet())
	{
		Layout.RegisterGraphOutputNode(onFinishedNodeHandle, FName("On Finished"));
	}

	// Register audio output(s)
	FMetaSoundNodeHandle audioLeftNodeHandle(outAudioHandles[0].NodeID);
	Layout.RegisterGraphOutputNode(audioLeftNodeHandle,
		FName(bisStereo ? "Audio Left" : "Audio Mono"));

	if (bisStereo && outAudioHandles.Num() > 1)
	{
		FMetaSoundNodeHandle audioRightNodeHandle(outAudioHandles[1].NodeID);
		Layout.RegisterGraphOutputNode(audioRightNodeHandle, FName("Audio Right"));
	}

	// Extract parameter name from container (use custom name from JSON, fallback to "Switch Parameter")
	FName SwitchParameterName = switchContainer->ParameterID.IsNone() ?
		FName("Switch Parameter") : switchContainer->ParameterID;

	// Create graph input for switch parameter
	FMetaSoundBuilderNodeOutputHandle parameterInputHandle = builder->AddGraphInputNode(
		SwitchParameterName,  // Use container's parameter name instead of hardcoded
		FName("String"),
		FMetasoundFrontendLiteral(switchContainer->DefaultParameterValue),
		result
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchSource: Failed to add Switch Parameter input for '%s'"), *SwitchParameterName.ToString());
		return nullptr;
	}

	// Register switch parameter with layout
	FMetaSoundNodeHandle switchParamNodeHandle(parameterInputHandle.NodeID);
	Layout.RegisterGraphInputNode(switchParamNodeHandle, SwitchParameterName);

	// ========================================================================
	// PHASE 2: Build Core Logic (from base class)
	// ========================================================================

	BuildSwitchCore(
		builder,
		switchContainer,
		Name,
		parameterInputHandle,
		OnPlayNode,
		OnFinishedNode,
		outAudioHandles[0],
		(bisStereo && outAudioHandles.Num() > 1) ? &outAudioHandles[1] : nullptr,
		&Layout
	);

	// ========================================================================
	// PHASE 3: Apply Layout and Build to Asset
	// ========================================================================

	// Compute and apply layout before building asset
	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> document = EditorSubsystem->BuildToAsset(
		builder, "ISX - Demute", Name, PathGlobal_Source, result
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchSource: BuildToAsset FAILED"));
		return nullptr;
	}

	UMetaSoundSource* source = Cast<UMetaSoundSource>(document.GetObject());
	UE_LOG(LogTemp, Log, TEXT("BuildSwitchSource: Successfully built for: %s"), *Name);

	return source;
}

// ============================================================================
// BuildTriggerSource - Build MetaSound source for Trigger container
// ============================================================================

UMetaSoundSource* UGISB_MetasoundSourceBuilder::BuildTriggerSource(
	UGisbImportContainerTrigger* triggerContainer,
	const FString& Name,
	bool canLoop
)
{
	if (!triggerContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTriggerSource: Invalid container"));
		return nullptr;
	}

	EMetaSoundBuilderResult result;

	// Determine output format (mono or stereo)
	bool bisStereo = triggerContainer->TriggeredSoundImport->bIsStereo;

	EMetaSoundOutputAudioFormat format = bisStereo ?
		EMetaSoundOutputAudioFormat::Stereo : EMetaSoundOutputAudioFormat::Mono;

	// ========================================================================
	// Create OnPlay and OnFinished nodes as references (filled by CreateSourceBuilder)
	// ========================================================================

	FMetaSoundBuilderNodeOutputHandle OnPlayNode;
	FMetaSoundBuilderNodeInputHandle OnFinishedNode;
	TArray<FMetaSoundBuilderNodeInputHandle> outAudioHandles;

	UMetaSoundSourceBuilder* builder = BuilderGlobal_Source->CreateSourceBuilder(
		FName(Name), OnPlayNode, OnFinishedNode, outAudioHandles, result, format, !canLoop
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTriggerSource: Failed to create source builder"));
		return nullptr;
	}

	builder->SetAuthor("ISX - Demute");

	// Create layout manager for node positioning
	GisbMetasoundLayoutManager Layout(builder, FGisbLayoutConfig::Spacious());

	// Extract node handles from builder handles
	FMetaSoundNodeHandle onPlayNodeHandle(OnPlayNode.NodeID);
	FMetaSoundNodeHandle onFinishedNodeHandle(OnFinishedNode.NodeID);

	// Register graph I/O nodes with layout
	Layout.RegisterGraphInputNode(onPlayNodeHandle, FName("Play"));
	if (OnFinishedNode.IsSet())
	{
		Layout.RegisterGraphOutputNode(onFinishedNodeHandle, FName("On Finished"));
	}

	// Register audio output(s)
	FMetaSoundNodeHandle audioLeftNodeHandle(outAudioHandles[0].NodeID);
	Layout.RegisterGraphOutputNode(audioLeftNodeHandle,
		FName(bisStereo ? "Audio Left" : "Audio Mono"));

	if (bisStereo && outAudioHandles.Num() > 1)
	{
		FMetaSoundNodeHandle audioRightNodeHandle(outAudioHandles[1].NodeID);
		Layout.RegisterGraphOutputNode(audioRightNodeHandle, FName("Audio Right"));
	}

	// ========================================================================
	// Call core build method
	// ========================================================================

	BuildTriggerCore(
		builder, triggerContainer, Name,
		OnPlayNode, OnFinishedNode,
		outAudioHandles[0], (bisStereo && outAudioHandles.Num() > 1) ? &outAudioHandles[1] : nullptr,
		&Layout
	);

	// ========================================================================
	// Apply layout and build source asset
	// ========================================================================

	Layout.ComputeLayout();
	Layout.ApplyLayout();

	UMetaSoundEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> document = EditorSubsystem->BuildToAsset(
		builder, "ISX - Demute", Name, PathGlobal_Source, result
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildTriggerSource: BuildToAsset FAILED"));
		return nullptr;
	}

	UMetaSoundSource* source = Cast<UMetaSoundSource>(document.GetObject());
	UE_LOG(LogTemp, Log, TEXT("BuildTriggerSource: Successfully built for: %s"), *Name);

	return source;
}
