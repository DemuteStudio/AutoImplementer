// Copyright Demute SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundSource.h"
#include "GISB_MetasoundBuilderCore.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerRandom.h"
#include "GisbImportContainerSwitch.h"
#include "GisbImportContainerBlend.h"
#include "GisbImportContainerTrigger.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundDocumentInterface.h"
#include "MetasoundBuilderSubsystem.h"
#include "MetasoundBuilderBase.h"
#include "MetasoundEditorSubsystem.h"
#include "GisbMetasoundLayoutManager.h"
#include "GISB_MetasoundNodeLibrary.h"
#include "GISB_MetasoundPatchBuilder.generated.h"

class UGisbSoundBankDataAsset;

// Pin metadata for parameter propagation
struct FGisbPinInfo
{
	FName PinName;
	FName PinType;  // "Float", "String", "Bool", "Int32", "Trigger", "Audio"

	FGisbPinInfo() : PinName(NAME_None), PinType(NAME_None) {}
	FGisbPinInfo(FName InName, FName InType) : PinName(InName), PinType(InType) {}
};

// Result struct for recursive child patch building
struct FChildPatchResult
{
	TScriptInterface<IMetaSoundDocumentInterface> Patch;
	UGisbImportContainerBase* Container;  // Source container (access cached properties via Container->bIsStereo)
	TArray<FGisbPinInfo> RequiredInputs;  // Parameters this child needs (e.g., switch parameter)

	FChildPatchResult() : Patch(nullptr), Container(nullptr) {}
};

UCLASS()
class GISB_IMPORTER_API UGISB_MetasoundPatchBuilder : public UGISB_MetasoundBuilderCore
{
	GENERATED_BODY()

public:
	// Test function for builder
	UFUNCTION(BlueprintCallable, Category = "ISX")
	static void TestBuilder(UGisbSoundBankDataAsset* dataAsset);

	// Recursive child builder with stereo detection (moved from private to public)
	// Can be called by SourceBuilder to build child patches
	static FChildPatchResult BuildChildNode(
		UGisbImportContainerBase* container,
		const FString& ParentName,
		int32 ChildIndex
	);

private:
	// Build simple sound node patch
	static TScriptInterface<IMetaSoundDocumentInterface> BuildSimpleSoundNode(
		UGisbImportContainerSimpleSound* simpleSound,
		const FString& Name
	);

	// Build random container node patch
	static TScriptInterface<IMetaSoundDocumentInterface> BuildRandomNode(
		UGisbImportContainerRandom* randomContainer,
		const FString& Name
	);

	// Build switch container node patch
	static TScriptInterface<IMetaSoundDocumentInterface> BuildSwitchNode(
		UGisbImportContainerSwitch* switchContainer,
		const FString& Name
	);

	// Build blend container node patch
	static TScriptInterface<IMetaSoundDocumentInterface> BuildBlendNode(
		UGisbImportContainerBlend* blendContainer,
		const FString& Name
	);

	// Build trigger container node patch
	static TScriptInterface<IMetaSoundDocumentInterface> BuildTriggerNode(
		UGisbImportContainerTrigger* triggerContainer,
		const FString& Name
	);

	// Note: Helper methods (setupProbability, setupAttributes, ConnectProbabilityNode,
	// ConnectVolume, ConnectPitch, ConnectLowpass, isStereo) are now inherited from
	// UGISB_MetasoundBuilderCore base class

};

// Global variables
extern FString PathGlobal;
extern UMetaSoundBuilderSubsystem* BuilderGlobal;
