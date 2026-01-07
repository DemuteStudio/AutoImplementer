// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundSource.h"
#include "GISB_MetasoundBuilderCore.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerBlend.h"
#include "GisbImportContainerRandom.h"
#include "GisbImportContainerSwitch.h"
#include "GisbImportContainerTrigger.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundDocumentInterface.h"
#include "MetasoundBuilderSubsystem.h"
#include "MetasoundBuilderBase.h"
#include "MetasoundEditorSubsystem.h"
#include "GISB_MetasoundNodeLibrary.h"
#include "GISB_MetasoundSourceBuilder.generated.h"

/**
 * MetaSound Source Builder - builds playable MetaSound sources.
 * Inherits shared helper methods from UGISB_MetasoundBuilderCore.
 */
UCLASS()
class UGISB_MetasoundSourceBuilder : public UGISB_MetasoundBuilderCore
{
	GENERATED_BODY()
public:

	// Test function for source builder - converts and builds from data asset
	UFUNCTION(BlueprintCallable, Category = "ISX")
	static UMetaSoundSource* TestBuilder(UGisbSoundBankDataAsset* dataAsset);

	UFUNCTION(BlueprintCallable, Category = "Metasound")
	static UMetaSoundSource* CreateMetasoundFromGISB(UGisbImportContainerBase* gisb, const FString& Name, const FString& path);

private:
	// New Build methods (4-phase pattern for sources)
	static UMetaSoundSource* BuildSimpleSource(
		UGisbImportContainerSimpleSound* simpleSound,
		const FString& Name,
		bool canLoop
	);

	static UMetaSoundSource* BuildRandomSource(
		UGisbImportContainerRandom* randomContainer,
		const FString& Name,
		bool canLoop
	);

	static UMetaSoundSource* BuildBlendSource(
		UGisbImportContainerBlend* blendContainer,
		const FString& Name,
		bool canLoop
	);

	static UMetaSoundSource* BuildSwitchSource(
		UGisbImportContainerSwitch* switchContainer,
		const FString& Name,
		bool canLoop
	);

	static UMetaSoundSource* BuildTriggerSource(
		UGisbImportContainerTrigger* triggerContainer,
		const FString& Name,
		bool canLoop
	);

	// Keep existing helper
	static void DetectLoopAndMono(UGisbImportContainerBase* gisb, bool& shouldLoop, bool& shouldStereo);
};
