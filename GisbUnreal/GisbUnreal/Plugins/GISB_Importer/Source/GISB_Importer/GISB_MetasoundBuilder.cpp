// Fill out your copyright notice in the Description page of Project Settings.


#include "GISB_MetasoundBuilder.h"
#include "MetasoundSource.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundBuilderSubsystem.h"
#include "MetasoundBuilderBase.h"
#include "MetasoundDocumentInterface.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "MetasoundEditorSubsystem.h"
#include <cmath>

FMetasoundFrontendClassName* UGISB_MetasoundBuilder::WavePlayerMonoNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundBuilder::WavePlayerStereoNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundBuilder::ProbabilityNode = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundBuilder::MonoMixerNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundBuilder::StereoMixerNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundBuilder::TriggerAnyNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundBuilder::TriggerAccumulateNodes = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundBuilder::AudioRerouteNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundBuilder::GisbRandomNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundBuilder::GisbSwitchNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundBuilder::GisbVolumeNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundBuilder::GisbPitchNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundBuilder::GisbLowpassNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundBuilder::GisbAttenuationNode = nullptr;

UMetaSoundSource* UGISB_MetasoundBuilder::CreateMetasoundFromGISB(UGisbImportContainerBase* gisb, const FString& Name, const FString& path)
{
	bool canLoop = false;
	bool shouldStereo = false;
	DetectLoopAndMono(gisb, canLoop, shouldStereo);
	EMetaSoundOutputAudioFormat format = shouldStereo ? EMetaSoundOutputAudioFormat::Stereo : EMetaSoundOutputAudioFormat::Mono;
	
	EMetaSoundBuilderResult result;
	UMetaSoundBuilderSubsystem* BuilderSubsystem = GEngine->GetEngineSubsystem<UMetaSoundBuilderSubsystem>();
	FMetaSoundBuilderNodeOutputHandle OnPlayNode;
	FMetaSoundBuilderNodeInputHandle OnFinishedNode;
	TArray<FMetaSoundBuilderNodeInputHandle> outAudioHandles;
	UMetaSoundSourceBuilder* builder = BuilderSubsystem->CreateSourceBuilder(FName(Name), OnPlayNode, OnFinishedNode, outAudioHandles, result,format, !canLoop);
	builder->SetAuthor("GISB");
	
	SetupNodes();
	ConnectContainerToGraph(builder, gisb, &OnPlayNode, &OnFinishedNode, &outAudioHandles);

	FMetaSoundBuilderOptions options = FMetaSoundBuilderOptions();
	UMetaSoundEditorSubsystem* MySubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> ms = MySubsystem->BuildToAsset(builder, "GISB", Name, path, result);
	if (result == EMetaSoundBuilderResult::Failed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to build UMetaSoundSource."))
	}
	UObject* Object = ms.GetObject();
	UMetaSoundSource* GeneratedMS = Cast<UMetaSoundSource>(Object);
	GeneratedMS->AttenuationSettings = gisb->Attenuation.soundAttenuation;
	return GeneratedMS;
}

void UGISB_MetasoundBuilder::SetupNodes()
{
	FName ue = TEXT("UE");
	FName wave = TEXT("Wave Player");

	UMetaSoundPatch* ReroutePatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_AudioReroute.GISB_AudioReroute")));
	AudioRerouteNode = TScriptInterface<IMetaSoundDocumentInterface>(ReroutePatch);

	WavePlayerMonoNode = new FMetasoundFrontendClassName(ue, wave, TEXT("Mono"));

	WavePlayerStereoNode = new FMetasoundFrontendClassName(ue, wave, TEXT("Stereo"));

	UMetaSoundPatch* AttenuationPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_Attenuation.GISB_Attenuation")));
	GisbAttenuationNode = TScriptInterface<IMetaSoundDocumentInterface>(AttenuationPatch);

	ProbabilityNode = new FMetasoundFrontendClassName(ue, TEXT("Trigger Filter"));

	UMetaSoundPatch* lowpassPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomLowpass.GISB_RandomLowpass")));
	GisbLowpassNode = TScriptInterface<IMetaSoundDocumentInterface>(lowpassPatch);

	UMetaSoundPatch* pitchPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomPitch.GISB_RandomPitch")));
	GisbPitchNode = TScriptInterface<IMetaSoundDocumentInterface>(pitchPatch);

	UMetaSoundPatch* randomPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomPlayer.GISB_RandomPlayer")));
	GisbRandomNode = TScriptInterface<IMetaSoundDocumentInterface>(randomPatch);

	UMetaSoundPatch* volumePatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomVolume.GISB_RandomVolume")));
	GisbVolumeNode = TScriptInterface<IMetaSoundDocumentInterface>(volumePatch);

	UMetaSoundPatch* switchPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_SwitchPlayer.GISB_SwitchPlayer")));
	GisbSwitchNode = TScriptInterface<IMetaSoundDocumentInterface>(switchPatch);

	FName mixer = TEXT("AudioMixer");
	FName any = TEXT("TriggerAny");
	FName accumulate = TEXT("TriggerAccumulator");

	MonoMixerNodes = new TArray<FMetasoundFrontendClassName*>;
	StereoMixerNodes = new TArray<FMetasoundFrontendClassName*>;
	TriggerAnyNodes = new TArray<FMetasoundFrontendClassName*>;
	TriggerAccumulateNodes = new TArray<FMetasoundFrontendClassName*>;

	TriggerAccumulateNodes->Add(new FMetasoundFrontendClassName(accumulate, TEXT("Trigger Accumulate (1)")));

	for (int i = 2; i <= 8; i++) //MS multi-input/output nodes start from 2 and end at 8    // Except for Trigger Accumulate which is 1
	{
        FName monoName = FName(*FString::Printf(TEXT("Audio Mixer (Mono, %d)"), i));
		FName stereoName = FName(*FString::Printf(TEXT("Audio Mixer (Stereo, %d)"), i));
		FName anyName = FName(*FString::Printf(TEXT("Trigger Any (%d)"), i));
		FName accumulateName = FName(*FString::Printf(TEXT("Trigger Accumulate (%d)"), i));

		FMetasoundFrontendClassName* monoNode = new FMetasoundFrontendClassName(mixer, monoName);
		FMetasoundFrontendClassName* stereoNode = new FMetasoundFrontendClassName(mixer, stereoName);
		FMetasoundFrontendClassName* triggerAnyNode = new FMetasoundFrontendClassName(any, anyName);
		FMetasoundFrontendClassName* triggerAccumulateNode = new FMetasoundFrontendClassName(accumulate, accumulateName);

		MonoMixerNodes->Add(monoNode);
		StereoMixerNodes->Add(stereoNode);
		TriggerAnyNodes->Add(triggerAnyNode);
		TriggerAccumulateNodes->Add(triggerAccumulateNode);
	}
}

void UGISB_MetasoundBuilder::DetectLoopAndMono(UGisbImportContainerBase* gisb, bool& canLoop, bool& shouldStereo)
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

void UGISB_MetasoundBuilder::ConnectContainerToGraph(UMetaSoundSourceBuilder* builder, UGisbImportContainerBase* gisb, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeInputHandle* finishHandle, TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles)
{
	EMetaSoundBuilderResult result;
	FMetaSoundBuilderNodeOutputHandle onFinishHandle;
	FMetaSoundBuilderNodeOutputHandle firstAudioHandle;
	FMetaSoundBuilderNodeOutputHandle secondAudioHandle;
	bool isStereo = audioHandles->Num() > 1;

	if (gisb->PlaybackProbabilityPercent < 99.99f)
		ConnectProbabilityNode(builder, gisb->PlaybackProbabilityPercent, *executionHandle, *finishHandle);

	UGisbImportContainerSimpleSound* simplesound = Cast<UGisbImportContainerSimpleSound>(gisb);
	if (simplesound != nullptr)
		ConnectSimpleSound(builder, simplesound, isStereo, executionHandle, onFinishHandle, firstAudioHandle, secondAudioHandle, audioHandles->Num());

	UGisbImportContainerBlend* blend = Cast<UGisbImportContainerBlend>(gisb);
	if (blend != nullptr)
		ConnectBlend(builder, blend, isStereo, executionHandle, onFinishHandle, firstAudioHandle, secondAudioHandle);

	UGisbImportContainerRandom* random = Cast<UGisbImportContainerRandom>(gisb);
	if (random != nullptr)
		ConnectRandom(builder, random, isStereo, executionHandle, onFinishHandle, firstAudioHandle, secondAudioHandle);

	UGisbImportContainerSwitch* swtch = Cast<UGisbImportContainerSwitch>(gisb);
	if (swtch != nullptr)
	{
		for (TPair<FString, TObjectPtr<UGisbImportContainerBase>>& pair : swtch->SoundDictionary)
		{
			UGisbImportContainerBase* newGisb = pair.Value;
			ConnectContainerToGraph(builder, newGisb, executionHandle, finishHandle, audioHandles);
		}
		// Create switch node at the beginning and at the end
		
	}

	if (&onFinishHandle == nullptr || &firstAudioHandle == nullptr) return; // if no cast was successful (or if not implemented), we don't have any audio to connect
	
	if(gisb->Attenuation.overrideParent) // will change the way the attenuation is checked
	{
		// TODO
	}

	if (gisb->VolumeDB.value != 0 || gisb->VolumeDB.randomize)
		ConnectVolume(builder, gisb->VolumeDB, isStereo, executionHandle, firstAudioHandle, secondAudioHandle, audioHandles);
		
	if(gisb->Pitch.value != 0 || gisb->Pitch.randomize)
		ConnectPitch(builder, gisb->Pitch, isStereo, executionHandle, firstAudioHandle, secondAudioHandle, audioHandles);

	if(gisb->Lowpass.value != 0 || gisb->Lowpass.randomize)
		ConnectLowpass(builder, gisb->Lowpass, isStereo, executionHandle, firstAudioHandle, secondAudioHandle, audioHandles);

	builder->ConnectNodes(onFinishHandle, *finishHandle, result);

	builder->ConnectNodes(firstAudioHandle, (*audioHandles)[0], result);
	if(audioHandles->Num() > 1 && isStereo)
		builder->ConnectNodes(secondAudioHandle, (*audioHandles)[1], result);
}

void UGISB_MetasoundBuilder::ConnectProbabilityNode(UMetaSoundSourceBuilder* builder, float probability, FMetaSoundBuilderNodeOutputHandle& executionHandle, FMetaSoundBuilderNodeInputHandle& finishHandle)
{
	EMetaSoundBuilderResult result;
	FMetaSoundNodeHandle probabilityHandle;
	probabilityHandle = builder->AddNodeByClassName(*ProbabilityNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded) return;
	
	FMetaSoundBuilderNodeInputHandle floatHandle = builder->FindNodeInputByName(probabilityHandle, TEXT("Probability"), result);
	FMetaSoundBuilderNodeInputHandle triggerHandle = builder->FindNodeInputByName(probabilityHandle, TEXT("Trigger"), result);

	FAudioParameter floatParam = FAudioParameter(TEXT("Probability"), probability * 0.01f);
	FMetasoundFrontendLiteral floatValue = FMetasoundFrontendLiteral(floatParam);
	builder->SetNodeInputDefault(floatHandle, floatValue, result);

	builder->ConnectNodes(executionHandle, triggerHandle, result);

	FMetaSoundBuilderNodeOutputHandle headsHandle = builder->FindNodeOutputByName(probabilityHandle, TEXT("Heads"), result);
	FMetaSoundBuilderNodeOutputHandle tailsHandle = builder->FindNodeOutputByName(probabilityHandle, TEXT("Tails"), result);

	executionHandle = headsHandle;

	FMetaSoundNodeHandle anyHandle;
	anyHandle = builder->AddNodeByClassName(*((*TriggerAnyNodes)[0]), result);
	if (result != EMetaSoundBuilderResult::Succeeded) return;
	
	FMetaSoundBuilderNodeInputHandle failHandle = builder->FindNodeInputByName(anyHandle, TEXT("In 1"), result);
	builder->ConnectNodes(tailsHandle, failHandle, result);

	FMetaSoundBuilderNodeOutputHandle outHandle = builder->FindNodeOutputByName(anyHandle, TEXT("Out"), result);
	builder->ConnectNodes(outHandle, finishHandle, result);

	FMetaSoundBuilderNodeInputHandle succesHandle = builder->FindNodeInputByName(anyHandle, TEXT("In 0"), result);
	finishHandle = succesHandle;
}

void UGISB_MetasoundBuilder::ConnectSimpleSound(UMetaSoundSourceBuilder* builder, UGisbImportContainerSimpleSound* simplesound, bool& isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& onFinishHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, const int32 audiHandlesNum)
{
	EMetaSoundBuilderResult result;
	if (simplesound->SoundWave && !isStereo) isStereo = simplesound->SoundWave->NumChannels > 1;
	FMetaSoundNodeHandle handle;
	handle = builder->AddNodeByClassName(isStereo ? *WavePlayerStereoNode : *WavePlayerMonoNode, result);

	if (result != EMetaSoundBuilderResult::Succeeded) return;
	FMetaSoundBuilderNodeInputHandle playHandle = builder->FindNodeInputByName(handle, TEXT("Play"), result);
	FMetaSoundBuilderNodeInputHandle waveAssetHandle = builder->FindNodeInputByName(handle, TEXT("Wave Asset"), result);
	FMetaSoundBuilderNodeInputHandle loopHandle = builder->FindNodeInputByName(handle, TEXT("Loop"), result);

	FAudioParameter waveParam = FAudioParameter(TEXT("Wave Asset"), simplesound->SoundWave);
	FMetasoundFrontendLiteral waveValue = FMetasoundFrontendLiteral(waveParam);
	builder->SetNodeInputDefault(waveAssetHandle, waveValue, result);

	FAudioParameter boolParam = FAudioParameter(TEXT("Loop"), simplesound->loop);
	FMetasoundFrontendLiteral loopValue = FMetasoundFrontendLiteral(boolParam);
	builder->SetNodeInputDefault(loopHandle, loopValue, result);

	builder->ConnectNodes(*executionHandle, playHandle, result);

	onFinishHandle = builder->FindNodeOutputByName(handle, TEXT("On Finished"), result);
	firstAudioHandle = builder->FindNodeOutputByName(handle, isStereo ? TEXT("Out Left") : TEXT("Out Mono"), result);

	if (isStereo || audiHandlesNum > 1) secondAudioHandle = builder->FindNodeOutputByName(handle, TEXT("Out Right"), result);
}

void UGISB_MetasoundBuilder::ConnectBlend(UMetaSoundSourceBuilder* builder, UGisbImportContainerBlend* blend, bool& isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& onFinishHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle)
{
	EMetaSoundBuilderResult result;
	bool canBlendLoop = false;
	if (!isStereo) DetectLoopAndMono(blend, canBlendLoop, isStereo);

	TArray<FMetaSoundBuilderNodeInputHandle> mixerAudioHandles;
	TArray<FMetaSoundBuilderNodeInputHandle> triggerAudioHandles;

	int numChildren = blend->SoundArray.Num();
	if (numChildren < 1) return;
	else if (numChildren == 1)
	{
		FMetaSoundNodeHandle rerouteHandle;
		rerouteHandle = builder->AddNode(AudioRerouteNode, result);

		onFinishHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutExec"), result);
		firstAudioHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutL"), result);
		if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutR"), result);

		FMetaSoundBuilderNodeInputHandle blendExecHandle = builder->FindNodeInputByName(rerouteHandle, TEXT("InExec"), result);
		TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles = new TArray<FMetaSoundBuilderNodeInputHandle>();
		audioHandles->Add(builder->FindNodeInputByName(rerouteHandle, TEXT("InL"), result));
		audioHandles->Add(builder->FindNodeInputByName(rerouteHandle, TEXT("InR"), result));

		ConnectContainerToGraph(builder, blend->SoundArray[0], executionHandle, &blendExecHandle, audioHandles);
		return;
	}
	else if (numChildren > 8)
	{
		int numMixerNodes = FMath::CeilToInt((float)numChildren / 7.0f);

		FMetaSoundBuilderNodeOutputHandle* firstEncapsulatedMixerHandle = nullptr;
		FMetaSoundBuilderNodeOutputHandle* secondEncapsulatedMixerHandle = nullptr;
		FMetaSoundBuilderNodeOutputHandle* encapsulatedTriggerHandle = nullptr;

		for (int m = 0; m < numMixerNodes; m++)
		{
			int newNumChildren = FMath::Min(numChildren - m * 7, 7) + 1; // We clamp at 7 (instead of 8) and then add 1 to leave room for the previous mixer node
			FMetaSoundNodeHandle mixerHandle;
			if (m != 0)
				mixerHandle = builder->AddNodeByClassName(*((isStereo ? *StereoMixerNodes : *MonoMixerNodes)[newNumChildren - 2]), result); // 2 inputs is array index 0, 8 inputs is array index 6
			else
				mixerHandle = builder->AddNodeByClassName(*((isStereo ? *StereoMixerNodes : *MonoMixerNodes)[newNumChildren - 3]), result); // Same as before but first spot is not used when it's the first mixer node

			FMetaSoundNodeHandle accumulateHandle;
			if (m != 0)
				accumulateHandle = builder->AddNodeByClassName(*((*TriggerAccumulateNodes)[newNumChildren - 1]), result); // 1 input is array index 0, 8 inputs is array index 7
			else
				accumulateHandle = builder->AddNodeByClassName(*((*TriggerAccumulateNodes)[newNumChildren - 2]), result); // Space 1 is free when it's the first accumulate


			if (m == numMixerNodes - 1)
			{
				onFinishHandle = builder->FindNodeOutputByName(accumulateHandle, TEXT("Out"), result);
				firstAudioHandle = builder->FindNodeOutputByName(mixerHandle, isStereo ? TEXT("Out L") : TEXT("Out"), result);
				if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(mixerHandle, TEXT("Out R"), result);
			}

			if (firstEncapsulatedMixerHandle != nullptr && encapsulatedTriggerHandle != nullptr)
			{
				if (isStereo && secondEncapsulatedMixerHandle != nullptr)
				{
					FMetaSoundBuilderNodeInputHandle inL = builder->FindNodeInputByName(mixerHandle, TEXT("In 0 L"), result);
					FMetaSoundBuilderNodeInputHandle inR = builder->FindNodeInputByName(mixerHandle, TEXT("In 0 R"), result);
					builder->ConnectNodes(*firstEncapsulatedMixerHandle, inL, result);
					builder->ConnectNodes(*secondEncapsulatedMixerHandle, inR, result);
				}
				else
				{
					FMetaSoundBuilderNodeInputHandle inAudio = builder->FindNodeInputByName(mixerHandle, TEXT("In 0"), result);
					builder->ConnectNodes(*firstEncapsulatedMixerHandle, inAudio, result);
				}

				FMetaSoundBuilderNodeInputHandle inTrigger = builder->FindNodeInputByName(accumulateHandle, TEXT("In 0"), result);
				builder->ConnectNodes(*encapsulatedTriggerHandle, inTrigger, result);
			}
			int start = m != 0 ? 1 : 0;
			int end = m != 0 ? newNumChildren : newNumChildren - 1; // We don't want to connect the first node again
			for (int i = start; i < end; i++) // Start at 1 because 0 is for the previous mixer
			{
				if (isStereo)
				{
					mixerAudioHandles.Add(builder->FindNodeInputByName(mixerHandle, *FString::Printf(TEXT("In %d L"), i), result));
					mixerAudioHandles.Add(builder->FindNodeInputByName(mixerHandle, *FString::Printf(TEXT("In %d R"), i), result));
				}
				else
				{
					mixerAudioHandles.Add(builder->FindNodeInputByName(mixerHandle, *FString::Printf(TEXT("In %d"), i), result));
				}
				triggerAudioHandles.Add(builder->FindNodeInputByName(accumulateHandle, *FString::Printf(TEXT("In %d"), i), result));
			}

			if (isStereo)
			{
				FMetaSoundBuilderNodeOutputHandle outL = builder->FindNodeOutputByName(mixerHandle, TEXT("Out L"), result);
				FMetaSoundBuilderNodeOutputHandle outR = builder->FindNodeOutputByName(mixerHandle, TEXT("Out R"), result);
				firstEncapsulatedMixerHandle = &outL;
				secondEncapsulatedMixerHandle = &outR;
			}
			else
			{
				FMetaSoundBuilderNodeOutputHandle outMono = builder->FindNodeOutputByName(mixerHandle, TEXT("Out"), result);
				firstEncapsulatedMixerHandle = &outMono;
			}

			FMetaSoundBuilderNodeOutputHandle outTrigger = builder->FindNodeOutputByName(accumulateHandle, TEXT("Out"), result);
			encapsulatedTriggerHandle = &outTrigger;
		}
	}
	else
	{
		FMetaSoundNodeHandle mixerHandle;
		mixerHandle = builder->AddNodeByClassName(*((isStereo ? *StereoMixerNodes : *MonoMixerNodes)[numChildren - 2]), result); // 2 inputs is array index 0, 8 inputs is array index 6

		FMetaSoundNodeHandle accumulateHandle;
		accumulateHandle = builder->AddNodeByClassName(*((*TriggerAccumulateNodes)[numChildren - 1]), result); // 1 input is array index 0, 8 inputs is array index 7

		onFinishHandle = builder->FindNodeOutputByName(accumulateHandle, TEXT("Out"), result);
		firstAudioHandle = builder->FindNodeOutputByName(mixerHandle, isStereo ? TEXT("Out L") : TEXT("Out"), result);
		if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(mixerHandle, TEXT("Out R"), result);

		for (int i = 0; i < numChildren; i++)
		{
			if (isStereo)
			{
				mixerAudioHandles.Add(builder->FindNodeInputByName(mixerHandle, *FString::Printf(TEXT("In %d L"), i), result));
				mixerAudioHandles.Add(builder->FindNodeInputByName(mixerHandle, *FString::Printf(TEXT("In %d R"), i), result));
			}
			else
			{
				mixerAudioHandles.Add(builder->FindNodeInputByName(mixerHandle, *FString::Printf(TEXT("In %d"), i), result));
			}
			triggerAudioHandles.Add(builder->FindNodeInputByName(accumulateHandle, *FString::Printf(TEXT("In %d"), i), result));
		}
	}

	for (int i = 0; i < numChildren; i++) // Connect each child container to the graph
	{
		UGisbImportContainerBase* newGisb = blend->SoundArray[i];

		TArray<FMetaSoundBuilderNodeInputHandle>* encaplsulatedAudioHandles = new TArray<FMetaSoundBuilderNodeInputHandle>();
		if (isStereo)
		{
			encaplsulatedAudioHandles->Add(mixerAudioHandles[i * 2]);
			encaplsulatedAudioHandles->Add(mixerAudioHandles[i * 2 + 1]);
		}
		else
		{
			encaplsulatedAudioHandles->Add(mixerAudioHandles[i]);
		}

		ConnectContainerToGraph(builder, newGisb, executionHandle, &triggerAudioHandles[i], encaplsulatedAudioHandles);
	}
}

void UGISB_MetasoundBuilder::ConnectRandom(UMetaSoundSourceBuilder* builder, UGisbImportContainerRandom* random, bool& isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& onFinishHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle)
{
	EMetaSoundBuilderResult result;
	bool canRandomLoop = false;
	bool unusedBool = false;
	DetectLoopAndMono(random, canRandomLoop, isStereo? unusedBool : isStereo);

	int numChildren = random->SoundArray.Num();

	//TODO : remove this hack when using configurable nodes
	if (numChildren > 8) numChildren = 8; // We limit the number of children to 8, because the nodes only support up to 8 inputs
	if (numChildren < 1) return;

	if (numChildren == 1) // No need to random one element
	{
		FMetaSoundNodeHandle rerouteHandle;
		rerouteHandle = builder->AddNode(AudioRerouteNode, result);

		onFinishHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutExec"), result);
		firstAudioHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutL"), result);
		if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(rerouteHandle, TEXT("OutR"), result);

		FMetaSoundBuilderNodeInputHandle randomExecHandle = builder->FindNodeInputByName(rerouteHandle, TEXT("InExec"), result);
		TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles = new TArray<FMetaSoundBuilderNodeInputHandle>();
		audioHandles->Add(builder->FindNodeInputByName(rerouteHandle, TEXT("InL"), result));
		audioHandles->Add(builder->FindNodeInputByName(rerouteHandle, TEXT("InR"), result));

		ConnectContainerToGraph(builder, random->SoundArray[0], executionHandle, &randomExecHandle, audioHandles);
		return;
	}

	TArray<FMetaSoundBuilderNodeInputHandle> mixerAudioHandles;
	TArray<FMetaSoundBuilderNodeInputHandle> triggerAudioHandles;

	TArray<FMetaSoundBuilderNodeOutputHandle> randomHandles;

	if (numChildren > 8)
	{
		int numMixerNodes = FMath::CeilToInt((float)numChildren / 8.0f);

		if (numMixerNodes > 8)
		{
			UE_LOG(LogTemp, Warning, TEXT("Too many children in random container. Max is 64, but found %d."), numChildren);
			numMixerNodes = 8;
		}

		int numChildrenPerMixer = FMath::CeilToInt((float)numChildren / numMixerNodes);

		FMetaSoundNodeHandle mainRandomHandle = builder->AddNode(GisbRandomNode, result);
		TArray<int> mainPossibilities;

		FMetaSoundNodeHandle mainMixerHandle = builder->AddNodeByClassName(*((isStereo ? *StereoMixerNodes : *MonoMixerNodes)[numChildrenPerMixer - 2]), result); // 2 inputs is array index 0, 8 inputs is array index 6
		FMetaSoundNodeHandle mainAnyHandle = builder->AddNodeByClassName(*((*TriggerAnyNodes)[numChildrenPerMixer - 2]), result);

		FMetaSoundBuilderNodeInputHandle mainNoRepeatHandle = builder->FindNodeInputByName(mainRandomHandle, TEXT("No Repeats"), result);
		FAudioParameter mainNoRepeatParam = FAudioParameter(TEXT("No Repeats"), numMixerNodes-1);
		FMetasoundFrontendLiteral mainNoRepeatValue = FMetasoundFrontendLiteral(mainNoRepeatParam);
		builder->SetNodeInputDefault(mainNoRepeatHandle, mainNoRepeatValue, result);

		for (int m = 0; m < numMixerNodes; m++)
		{
			mainPossibilities.Add(m);
			FMetaSoundNodeHandle randomHandle = builder->AddNode(GisbRandomNode, result);

			int newNumChildren = numChildrenPerMixer;
			if (m == numMixerNodes - 1)
			{
				newNumChildren = numChildren - m * numChildrenPerMixer; // Last node gets the rest of the children
			}

			FMetaSoundBuilderNodeInputHandle noRepeatHandle = builder->FindNodeInputByName(randomHandle, TEXT("No Repeats"), result);
			FAudioParameter noRepeatParam = FAudioParameter(TEXT("No Repeats"), random->AvoidLastPlayed / numMixerNodes);
			FMetasoundFrontendLiteral noRepeatValue = FMetasoundFrontendLiteral(noRepeatParam);
			builder->SetNodeInputDefault(noRepeatHandle, noRepeatValue, result);

			TArray<int> possibilities;
			for (int i = 0; i < newNumChildren; i++) possibilities.Add(i);
			
			FMetaSoundBuilderNodeInputHandle possibilitiesHandle = builder->FindNodeInputByName(randomHandle, TEXT("Possibilities"), result);
			FAudioParameter possibilitiesParam = FAudioParameter(TEXT("Possibilities"), possibilities);
			FMetasoundFrontendLiteral possibilitiesValue = FMetasoundFrontendLiteral(possibilitiesParam);
			builder->SetNodeInputDefault(possibilitiesHandle, possibilitiesValue, result);

			// TODO: Connect the random nodes to the main random node, the mixer nodes to the main mixer node and the any nodes to the main any node
			// TODO: Add data to mixerAudioHandles, randomHandles and triggerAudioHandles
		}
		FMetaSoundBuilderNodeInputHandle mainPossibilitiesHandle = builder->FindNodeInputByName(mainRandomHandle, TEXT("Possibilities"), result);
		FAudioParameter mainPossibilitiesParam = FAudioParameter(TEXT("Possibilities"), mainPossibilities);
		FMetasoundFrontendLiteral mainPossibilitiesValue = FMetasoundFrontendLiteral(mainPossibilitiesParam);
		builder->SetNodeInputDefault(mainPossibilitiesHandle, mainPossibilitiesValue, result);
	}
	else
	{
		FMetaSoundNodeHandle mixerHandle;
		mixerHandle = builder->AddNodeByClassName(*((isStereo ? *StereoMixerNodes : *MonoMixerNodes)[numChildren - 2]), result); // 2 inputs is array index 0, 8 inputs is array index 6

		FMetaSoundNodeHandle anyHandle;
		anyHandle = builder->AddNodeByClassName(*((*TriggerAnyNodes)[numChildren - 2]), result); // same as above

		FMetaSoundNodeHandle randomHandle;
		randomHandle = builder->AddNode(GisbRandomNode, result);

		FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(randomHandle, TEXT("Exec"), result);
		builder->ConnectNodes(*executionHandle, execHandle, result);

		FMetaSoundBuilderNodeInputHandle noRepeatHandle = builder->FindNodeInputByName(randomHandle, TEXT("No Repeats"), result);
		FAudioParameter noRepeatParam = FAudioParameter(TEXT("No Repeats"), random->AvoidLastPlayed);
		FMetasoundFrontendLiteral noRepeatValue = FMetasoundFrontendLiteral(noRepeatParam);
		builder->SetNodeInputDefault(noRepeatHandle, noRepeatValue, result);

		TArray<int> possibilities;
		for (int i = 0; i < numChildren; i++) possibilities.Add(i);

		FMetaSoundBuilderNodeInputHandle possibilitiesHandle = builder->FindNodeInputByName(randomHandle, TEXT("Possibilities"), result);
		FAudioParameter possibilitiesParam = FAudioParameter(TEXT("Possibilities"), possibilities);
		FMetasoundFrontendLiteral possibilitiesValue = FMetasoundFrontendLiteral(possibilitiesParam);
		builder->SetNodeInputDefault(possibilitiesHandle, possibilitiesValue, result);

		onFinishHandle = builder->FindNodeOutputByName(anyHandle, TEXT("Out"), result);
		firstAudioHandle = builder->FindNodeOutputByName(mixerHandle, isStereo ? TEXT("Out L") : TEXT("Out"), result);
		if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(mixerHandle, TEXT("Out R"), result);

		for (int i = 0; i < numChildren; i++)
		{
			if (isStereo)
			{
				mixerAudioHandles.Add(builder->FindNodeInputByName(mixerHandle, *FString::Printf(TEXT("In %d L"), i), result));
				mixerAudioHandles.Add(builder->FindNodeInputByName(mixerHandle, *FString::Printf(TEXT("In %d R"), i), result));
			}
			else
			{
				mixerAudioHandles.Add(builder->FindNodeInputByName(mixerHandle, *FString::Printf(TEXT("In %d"), i), result));
			}
			triggerAudioHandles.Add(builder->FindNodeInputByName(anyHandle, *FString::Printf(TEXT("In %d"), i), result));
			randomHandles.Add(builder->FindNodeOutputByName(randomHandle, *FString::Printf(TEXT("Out %d"), i), result));
		}
	}

	for (int i = 0; i < numChildren; i++) //Connect each contained element
	{
		UGisbImportContainerBase* newGisb = random->SoundArray[i];

		TArray<FMetaSoundBuilderNodeInputHandle>* encaplsulatedAudioHandles = new TArray<FMetaSoundBuilderNodeInputHandle>();
		if (isStereo)
		{
			encaplsulatedAudioHandles->Add(mixerAudioHandles[i * 2]);
			encaplsulatedAudioHandles->Add(mixerAudioHandles[i * 2 + 1]);
		}
		else
		{
			encaplsulatedAudioHandles->Add(mixerAudioHandles[i]);
		}

		ConnectContainerToGraph(builder, newGisb, &randomHandles[i], &triggerAudioHandles[i], encaplsulatedAudioHandles);
	}
}

void UGISB_MetasoundBuilder::ConnectVolume(UMetaSoundSourceBuilder* builder, const FGisbVolume volume,const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles)
{
	EMetaSoundBuilderResult result;
	FMetaSoundNodeHandle volumeHandle;
	volumeHandle = builder->AddNode(GisbVolumeNode, result);
	if (result != EMetaSoundBuilderResult::Succeeded) return;
	
	FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(volumeHandle, TEXT("Exec"), result);
	FMetaSoundBuilderNodeInputHandle audioLeftHandle = builder->FindNodeInputByName(volumeHandle, TEXT("InL"), result);
	FMetaSoundBuilderNodeInputHandle audioRightHandle = builder->FindNodeInputByName(volumeHandle, TEXT("InR"), result);
	FMetaSoundBuilderNodeInputHandle maxHandle = builder->FindNodeInputByName(volumeHandle, TEXT("Max"), result);
	FMetaSoundBuilderNodeInputHandle minHandle = builder->FindNodeInputByName(volumeHandle, TEXT("Min"), result);
	FMetaSoundBuilderNodeInputHandle randomizeHandle = builder->FindNodeInputByName(volumeHandle, TEXT("Randomize"), result);
	FMetaSoundBuilderNodeInputHandle valueHandle = builder->FindNodeInputByName(volumeHandle, TEXT("Value"), result);

	builder->ConnectNodes(*executionHandle, execHandle, result);
	builder->ConnectNodes(firstAudioHandle, audioLeftHandle, result);
	if (isStereo) builder->ConnectNodes(secondAudioHandle, audioRightHandle, result);

	float linearMax = std::pow(10.0f, volume.maxRnd * 0.05f);
	FAudioParameter maxParam = FAudioParameter(TEXT("Max"), linearMax);
	FMetasoundFrontendLiteral maxValue = FMetasoundFrontendLiteral(maxParam);
	builder->SetNodeInputDefault(maxHandle, maxValue, result);

	float linearMin = std::pow(10.0f, volume.minRnd * 0.05f);
	FAudioParameter minParam = FAudioParameter(TEXT("Min"), linearMin);
	FMetasoundFrontendLiteral minValue = FMetasoundFrontendLiteral(minParam);
	builder->SetNodeInputDefault(minHandle, minValue, result);

	FAudioParameter randomizeParam = FAudioParameter(TEXT("Randomize"), volume.randomize);
	FMetasoundFrontendLiteral randomizeValue = FMetasoundFrontendLiteral(randomizeParam);
	builder->SetNodeInputDefault(randomizeHandle, randomizeValue, result);

	float linearValue = std::pow(10.0f, volume.value * 0.05f);
	FAudioParameter valueParam = FAudioParameter(TEXT("Value"), linearValue);
	FMetasoundFrontendLiteral valueValue = FMetasoundFrontendLiteral(valueParam);
	builder->SetNodeInputDefault(valueHandle, valueValue, result);

	firstAudioHandle = builder->FindNodeOutputByName(volumeHandle, TEXT("OutL"), result);
	if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(volumeHandle, TEXT("OutR"), result);
}

void UGISB_MetasoundBuilder::ConnectPitch(UMetaSoundSourceBuilder* builder, const FGisbPitch pitch, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles)
{
	EMetaSoundBuilderResult result;
	FMetaSoundNodeHandle pitchHandle;
	pitchHandle = builder->AddNode(GisbPitchNode, result);
	if (result == EMetaSoundBuilderResult::Succeeded)
	{
		FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(pitchHandle, TEXT("Exec"), result);
		FMetaSoundBuilderNodeInputHandle audioLeftHandle = builder->FindNodeInputByName(pitchHandle, TEXT("InL"), result);
		FMetaSoundBuilderNodeInputHandle audioRightHandle = builder->FindNodeInputByName(pitchHandle, TEXT("InR"), result);
		FMetaSoundBuilderNodeInputHandle maxHandle = builder->FindNodeInputByName(pitchHandle, TEXT("Max"), result);
		FMetaSoundBuilderNodeInputHandle minHandle = builder->FindNodeInputByName(pitchHandle, TEXT("Min"), result);
		FMetaSoundBuilderNodeInputHandle randomizeHandle = builder->FindNodeInputByName(pitchHandle, TEXT("Randomize"), result);
		FMetaSoundBuilderNodeInputHandle valueHandle = builder->FindNodeInputByName(pitchHandle, TEXT("Value"), result);

		builder->ConnectNodes(*executionHandle, execHandle, result);
		builder->ConnectNodes(firstAudioHandle, audioLeftHandle, result);
		if (isStereo) builder->ConnectNodes(secondAudioHandle, audioRightHandle, result);

		float linearMax = std::pow(2.0f, pitch.maxRnd * 0.00083333f);
		FAudioParameter maxParam = FAudioParameter(TEXT("Min"), linearMax);
		FMetasoundFrontendLiteral maxValue = FMetasoundFrontendLiteral(maxParam);
		builder->SetNodeInputDefault(maxHandle, maxValue, result);

		float linearMin = std::pow(2.0f, pitch.minRnd * 0.00083333f);
		FAudioParameter minParam = FAudioParameter(TEXT("Min"), linearMin);
		FMetasoundFrontendLiteral minValue = FMetasoundFrontendLiteral(minParam);
		builder->SetNodeInputDefault(minHandle, minValue, result);

		FAudioParameter randomizeParam = FAudioParameter(TEXT("Randomize"), pitch.randomize);
		FMetasoundFrontendLiteral randomizeValue = FMetasoundFrontendLiteral(randomizeParam);
		builder->SetNodeInputDefault(randomizeHandle, randomizeValue, result);

		float linearValue = std::pow(2.0f, pitch.value * 0.00083333f);
		FAudioParameter valueParam = FAudioParameter(TEXT("Value"), linearValue);
		FMetasoundFrontendLiteral valueValue = FMetasoundFrontendLiteral(valueParam);
		builder->SetNodeInputDefault(valueHandle, valueValue, result);

		firstAudioHandle = builder->FindNodeOutputByName(pitchHandle, TEXT("OutL"), result);
		if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(pitchHandle, TEXT("OutR"), result);
	}
}

void UGISB_MetasoundBuilder::ConnectLowpass(UMetaSoundSourceBuilder* builder, const FGisbLowPass lowpass, const bool isStereo, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeOutputHandle& firstAudioHandle, FMetaSoundBuilderNodeOutputHandle& secondAudioHandle, TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles)
{
	EMetaSoundBuilderResult result;
	FMetaSoundNodeHandle lowpassHandle;
	lowpassHandle = builder->AddNode(GisbLowpassNode, result);
	if (result == EMetaSoundBuilderResult::Succeeded)
	{
		FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(lowpassHandle, TEXT("Exec"), result);
		FMetaSoundBuilderNodeInputHandle audioLeftHandle = builder->FindNodeInputByName(lowpassHandle, TEXT("InL"), result);
		FMetaSoundBuilderNodeInputHandle audioRightHandle = builder->FindNodeInputByName(lowpassHandle, TEXT("InR"), result);
		FMetaSoundBuilderNodeInputHandle maxHandle = builder->FindNodeInputByName(lowpassHandle, TEXT("Max"), result);
		FMetaSoundBuilderNodeInputHandle minHandle = builder->FindNodeInputByName(lowpassHandle, TEXT("Min"), result);
		FMetaSoundBuilderNodeInputHandle randomizeHandle = builder->FindNodeInputByName(lowpassHandle, TEXT("Randomize"), result);
		FMetaSoundBuilderNodeInputHandle valueHandle = builder->FindNodeInputByName(lowpassHandle, TEXT("Value"), result);

		builder->ConnectNodes(*executionHandle, execHandle, result);
		builder->ConnectNodes(firstAudioHandle, audioLeftHandle, result);
		if (isStereo) builder->ConnectNodes(secondAudioHandle, audioRightHandle, result);


		//21980
		float frequencyMax = lowpass.maxRnd * 219.80;
		FAudioParameter maxParam = FAudioParameter(TEXT("Min"), frequencyMax);
		FMetasoundFrontendLiteral maxValue = FMetasoundFrontendLiteral(maxParam);
		builder->SetNodeInputDefault(maxHandle, maxValue, result);

		float frequencyMin = lowpass.minRnd * 219.80;
		FAudioParameter minParam = FAudioParameter(TEXT("Min"), frequencyMin);
		FMetasoundFrontendLiteral minValue = FMetasoundFrontendLiteral(minParam);
		builder->SetNodeInputDefault(minHandle, minValue, result);

		FAudioParameter randomizeParam = FAudioParameter(TEXT("Randomize"), lowpass.randomize);
		FMetasoundFrontendLiteral randomizeValue = FMetasoundFrontendLiteral(randomizeParam);
		builder->SetNodeInputDefault(randomizeHandle, randomizeValue, result);

		// 0 is 22 000
		// 100 is 20
		float frequencyValue = (100.0f - FMath::Clamp(lowpass.value, 0.0f, 100.0f)) * 219.80 + 20;
		FAudioParameter valueParam = FAudioParameter(TEXT("Value"), frequencyValue);
		FMetasoundFrontendLiteral valueValue = FMetasoundFrontendLiteral(valueParam);
		builder->SetNodeInputDefault(valueHandle, valueValue, result);

		firstAudioHandle = builder->FindNodeOutputByName(lowpassHandle, TEXT("OutL"), result);
		if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(lowpassHandle, TEXT("OutR"), result);
	}
}