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

// Global variables for source builder
FString PathGlobal_Source;
UMetaSoundBuilderSubsystem* BuilderGlobal_Source;

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
	UGISB_MetasoundNodeLibrary::SetupNodes();

	// Detect stereo and looping requirements
	bool shouldLoop = false;
	bool shouldStereo = false;
	DetectLoopAndMono(gisb, shouldLoop, shouldStereo);

	// Dispatch to appropriate Build method based on container type
	UMetaSoundSource* source = nullptr;

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
		nullptr               // No layout for sources
	);

	// ========================================================================
	// PHASE 3: Build to Asset (no layout needed for sources)
	// ========================================================================

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
	bool bisStereo = isStereo(randomContainer);
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

	// Build child patches (reuse PatchBuilder's BuildChildNode!)
	TArray<FChildPatchResult> childResults;
	for (int32 i = 0; i < randomContainer->SoundArray.Num(); i++)
	{
		FChildPatchResult childResult = UGISB_MetasoundPatchBuilder::BuildChildNode(
			randomContainer->SoundArray[i],
			Name,
			i
		);

		if (!childResult.Patch)
		{
			UE_LOG(LogTemp, Error, TEXT("BuildRandomSource: Failed to build child %d"), i);
			continue;
		}

		childResults.Add(childResult);
	}

	if (childResults.Num() < 1)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildRandomSource: No valid children"));
		return nullptr;
	}

	// ========================================================================
	// PHASE 2: Build Core Logic (from base class)
	// BuildCore connects directly to graph outputs (Pattern A)
	// ========================================================================

	BuildRandomCore(
		builder,                                      // Source builder
		randomContainer,                              // Container object
		Name,                                         // Base name
		OnPlayNode,                                   // Trigger (auto-created)
		OnFinishedNode,                               // On finished (auto-created)
		outAudioHandles[0],                           // Graph output left/mono (BuildCore connects automatically)
		(bisStereo && outAudioHandles.Num() > 1) ? &outAudioHandles[1] : nullptr,  // Graph output right (nullable)
		nullptr                                       // No layout
	);

	// ========================================================================
	// PHASE 3: Build to Asset
	// ========================================================================

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
	bool bisStereo = isStereo(blendContainer);
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
		nullptr
	);

	// ========================================================================
	// PHASE 3: Build to Asset
	// ========================================================================

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
	bool bisStereo = isStereo(switchContainer);
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

	// Create graph input for switch parameter
	FMetaSoundBuilderNodeOutputHandle parameterInputHandle = builder->AddGraphInputNode(
		FName("Switch Parameter"),
		FName("String"),
		FMetasoundFrontendLiteral(switchContainer->DefaultParameterValue),
		result
	);

	if (result != EMetaSoundBuilderResult::Succeeded)
	{
		UE_LOG(LogTemp, Error, TEXT("BuildSwitchSource: Failed to add Switch Parameter input"));
		return nullptr;
	}

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
		nullptr
	);

	// ========================================================================
	// PHASE 3: Build to Asset
	// ========================================================================

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
