// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetasoundSource.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerBlend.h"
#include "GisbImportContainerRandom.h"
#include "GisbImportContainerSwitch.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundDocumentInterface.h"
#include "MetasoundBuilderSubsystem.h"
#include "MetasoundBuilderBase.h"
#include "MetasoundEditorSubsystem.h"
#include "GISB_MetasoundBuilder.generated.h"

/**
 * 
 */
UCLASS()
class UGISB_MetasoundBuilder : public UObject
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable, Category = "Metasound")
	static UMetaSoundSource* CreateMetasoundFromGISB(UGisbImportContainerBase* gisb, const FString& Name, const FString& path);

private:
	static FMetasoundFrontendClassName* WavePlayerMonoNode;
	static FMetasoundFrontendClassName* WavePlayerStereoNode;
	static FMetasoundFrontendClassName* ProbabilityNode;
	static TArray<FMetasoundFrontendClassName*>* MonoMixerNodes;
	static TArray<FMetasoundFrontendClassName*>* StereoMixerNodes;
	static TArray<FMetasoundFrontendClassName*>* TriggerAnyNodes;
	static TArray<FMetasoundFrontendClassName*>* TriggerAccumulateNodes;
	static TScriptInterface<IMetaSoundDocumentInterface> AudioRerouteNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbRandomNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbSwitchNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbVolumeNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbPitchNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbLowpassNode;
	static TScriptInterface<IMetaSoundDocumentInterface> GisbAttenuationNode;
	static void SetupNodes();
	static void DetectLoopAndMono(UGisbImportContainerBase* gisb, bool& shouldLoop, bool& shouldStereo);
	static void ConnectContainerToGraph(UMetaSoundSourceBuilder* builder, UGisbImportContainerBase* gisb, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeInputHandle* finishHandle, TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles);
	static void ConnectProbabilityNode(UMetaSoundSourceBuilder* builder, float probability, FMetaSoundBuilderNodeOutputHandle& executionHandle, FMetaSoundBuilderNodeInputHandle& finishHandle);
	static void ConnectSimpleSound(UMetaSoundSourceBuilder* builder, UGisbImportContainerSimpleSound* simplesound, bool& isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& onFinishHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, const int32 audiHandlesNum);
	static void ConnectBlend(UMetaSoundSourceBuilder* builder, UGisbImportContainerBlend* blend, bool& isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& onFinishHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle);
	static void ConnectRandom(UMetaSoundSourceBuilder* builder, UGisbImportContainerRandom* random, bool& isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& onFinishHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle);
	static void ConnectVolume(UMetaSoundSourceBuilder* builder, const FGisbVolume volume, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles);
	static void ConnectPitch(UMetaSoundSourceBuilder* builder, const FGisbPitch pitch, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles);
	static void ConnectLowpass(UMetaSoundSourceBuilder* builder, const FGisbLowPass lowpass, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles);

	//New Multiple Node Implementation
	static TScriptInterface<IMetaSoundDocumentInterface> BuildSimpleSoundNode(UGisbImportContainerSimpleSound* simpleSound, const FString& Name);
};
