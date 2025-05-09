// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundSource.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerBlend.h"
#include "GisbImportContainerRandom.h"
#include "GisbImportContainerSwitch.h"
#include "MetasoundBuilderSubsystem.h"
#include "GISB_MetasoundBuilder.generated.h"

/**
 * 
 */
UCLASS()
class GISBUNREAL_API UGISB_MetasoundBuilder : public UObject
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "Metasound")
	static UMetaSoundSource* CreateMetasoundFromGISB(UGisbImportContainerBase* gisb, const FString& Name, const FString& path);

private:
	static FMetasoundFrontendClassName WavePlayerMonoNode;
	static FMetasoundFrontendClassName WavePlayerStereoNode;
	static FMetasoundFrontendClassName GisbRandomNode;
	static FMetasoundFrontendClassName GisbSwitchNode;
	static FMetasoundFrontendClassName GisbProbabilityNode;
	static FMetasoundFrontendClassName GisbVolumeNode;
	static FMetasoundFrontendClassName GisbPitchNode;
	static FMetasoundFrontendClassName GisbLowpassNode;
	static FMetasoundFrontendClassName GisbAttenuationNode;
	static TArray<FMetasoundFrontendClassName> MixerNodes;
	static TArray<FMetasoundFrontendClassName> TriggerAnyNodes;
	static TArray<FMetasoundFrontendClassName> TriggerAccumulateNodes;
	static void SetupNodes();
	static void DetectLoopAndMono(UGisbImportContainerBase* gisb, bool& shouldLoop, bool& shouldStereo);
	static void ConnectContainerToGraph(UMetaSoundSourceBuilder* builder, UGisbImportContainerBase* gisb, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeInputHandle* finishHandle, TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles);
};
