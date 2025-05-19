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
	{

		FMetaSoundNodeHandle probabilityHandle;
		probabilityHandle = builder->AddNodeByClassName(*ProbabilityNode, result);
		if (result == EMetaSoundBuilderResult::Succeeded)
		{
			FMetaSoundBuilderNodeInputHandle floatHandle = builder->FindNodeInputByName(probabilityHandle, TEXT("Probability"), result);
			FMetaSoundBuilderNodeInputHandle triggerHandle = builder->FindNodeInputByName(probabilityHandle, TEXT("Trigger"), result);

			FAudioParameter floatParam = FAudioParameter(TEXT("Probability"), gisb->PlaybackProbabilityPercent * 0.01f);
			FMetasoundFrontendLiteral floatValue = FMetasoundFrontendLiteral(floatParam);
			builder->SetNodeInputDefault(floatHandle, floatValue, result);

			builder->ConnectNodes(*executionHandle, triggerHandle, result);

			FMetaSoundBuilderNodeOutputHandle headsHandle = builder->FindNodeOutputByName(probabilityHandle, TEXT("Heads"), result);
			FMetaSoundBuilderNodeOutputHandle tailsHandle = builder->FindNodeOutputByName(probabilityHandle, TEXT("Tails"), result);
			
			executionHandle = &headsHandle;

			FMetaSoundNodeHandle anyHandle;
			anyHandle = builder->AddNodeByClassName(*((*TriggerAnyNodes)[0]), result);
			if(result == EMetaSoundBuilderResult::Succeeded)
			{
				FMetaSoundBuilderNodeInputHandle failHandle = builder->FindNodeInputByName(anyHandle, TEXT("In 1"), result);
				builder->ConnectNodes(tailsHandle, failHandle, result);

				FMetaSoundBuilderNodeOutputHandle outHandle = builder->FindNodeOutputByName(anyHandle, TEXT("Out"), result);
				builder->ConnectNodes(outHandle, *finishHandle, result);

				FMetaSoundBuilderNodeInputHandle succesHandle = builder->FindNodeInputByName(anyHandle, TEXT("In 0"), result);
				finishHandle = &succesHandle;
			}
		}
	}

	UGisbImportContainerSimpleSound* simplesound = Cast<UGisbImportContainerSimpleSound>(gisb);
	if (simplesound != nullptr)
	{
		if(simplesound->SoundWave && !isStereo) isStereo = simplesound->SoundWave->NumChannels > 1;
		FMetaSoundNodeHandle handle;
		handle = builder->AddNodeByClassName(isStereo ? *WavePlayerStereoNode : *WavePlayerMonoNode, result);
		if (result == EMetaSoundBuilderResult::Succeeded)
		{
			FMetaSoundBuilderNodeInputHandle playHandle = builder->FindNodeInputByName(handle,TEXT("Play"),result);
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
			
			if (isStereo || audioHandles->Num()>1) secondAudioHandle = builder->FindNodeOutputByName(handle, TEXT("Out Right"), result);
		}
	}

	UGisbImportContainerBlend* blend = Cast<UGisbImportContainerBlend>(gisb);
	if (blend != nullptr)
	{
		bool canBlendLoop = false;
		bool shouldContinue = true;
		if(!isStereo) DetectLoopAndMono(blend, canBlendLoop, isStereo);

		TArray<FMetaSoundBuilderNodeInputHandle> mixerAudioHandles;
		TArray<FMetaSoundBuilderNodeInputHandle> triggerAudioHandles;

		int numChildren = blend->SoundArray.Num();
		if (numChildren < 1)
		{
			return;
		}
		else if (numChildren == 1)
		{
			shouldContinue = false;
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
				if(m != 0)
					mixerHandle = builder->AddNodeByClassName(*((isStereo ? *StereoMixerNodes : *MonoMixerNodes)[newNumChildren - 2]), result); // 2 inputs is array index 0, 8 inputs is array index 6
				else
					mixerHandle = builder->AddNodeByClassName(*((isStereo ? *StereoMixerNodes : *MonoMixerNodes)[newNumChildren - 3]), result); // Same as before but first spot is not used when it's the first mixer node

				FMetaSoundNodeHandle accumulateHandle;
				if (m != 0)
					accumulateHandle = builder->AddNodeByClassName(*((*TriggerAccumulateNodes)[newNumChildren - 1]), result); // 1 input is array index 0, 8 inputs is array index 7
				else
					accumulateHandle = builder->AddNodeByClassName(*((*TriggerAccumulateNodes)[newNumChildren - 2]), result); // Space 1 is free when it's the first accumulate
				

				if(m == numMixerNodes - 1)
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
			mixerHandle = builder->AddNodeByClassName(*((isStereo ? *StereoMixerNodes : *MonoMixerNodes)[numChildren-2]), result); // 2 inputs is array index 0, 8 inputs is array index 6

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

		if (shouldContinue)
		{
			for (int i = 0; i < numChildren; i++)
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
	}

	UGisbImportContainerRandom* random = Cast<UGisbImportContainerRandom>(gisb);
	if (random != nullptr)
	{
		bool canRandomLoop = false;
		bool shouldContinue = true;
		if (!isStereo) DetectLoopAndMono(random, canRandomLoop, isStereo);

		TArray<FMetaSoundBuilderNodeInputHandle> mixerAudioHandles;
		TArray<FMetaSoundBuilderNodeInputHandle> triggerAudioHandles;

		TArray<FMetaSoundBuilderNodeOutputHandle> randomHandles;

		int numChildren = random->SoundArray.Num();
		if (numChildren < 1)
		{
			return;
		}
		else if (numChildren == 1)
		{
			shouldContinue = false;
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

				FMetaSoundNodeHandle randomHandle;
				randomHandle = builder->AddNode(GisbRandomNode, result);

				FMetaSoundBuilderNodeInputHandle execHandle = builder->FindNodeInputByName(randomHandle, TEXT("Exec"), result);
				builder->ConnectNodes(*executionHandle, execHandle, result);

				FMetaSoundBuilderNodeInputHandle noRepeatHandle = builder->FindNodeInputByName(randomHandle, TEXT("No Repeats"), result);
				FAudioParameter noRepeatParam = FAudioParameter(TEXT("No Repeats"), random->AvoidLastPlayed);
				FMetasoundFrontendLiteral noRepeatValue = FMetasoundFrontendLiteral(noRepeatParam);
				builder->SetNodeInputDefault(noRepeatHandle, noRepeatValue, result);

				TArray<int> possibilities;
				for (int i = 0; i < numChildren; i++)
				{
					possibilities.Add(i);
				}

				FMetaSoundBuilderNodeInputHandle possibilitiesHandle = builder->FindNodeInputByName(randomHandle, TEXT("Possibilities"), result);
				FAudioParameter possibilitiesParam = FAudioParameter(TEXT("Possibilities"), possibilities);
				FMetasoundFrontendLiteral possibilitiesValue = FMetasoundFrontendLiteral(possibilitiesParam);
				builder->SetNodeInputDefault(possibilitiesHandle, possibilitiesValue, result);

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
			for (int i = 0; i < numChildren; i++)
			{
				possibilities.Add(i);
			}

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

		if (shouldContinue)
		{
			for (int i = 0; i < numChildren; i++)
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
		
	}

	UGisbImportContainerSwitch* swtch = Cast<UGisbImportContainerSwitch>(gisb);
	if (swtch != nullptr)
	{
		for (TPair<FString, UGisbImportContainerBase*>& pair : swtch->SoundDictionary)
		{
			UGisbImportContainerBase* newGisb = pair.Value;
			ConnectContainerToGraph(builder, newGisb, executionHandle, finishHandle, audioHandles);
		}
		// Create switch node at the beginning and at the end
		
	}

	if (&onFinishHandle == nullptr || &firstAudioHandle == nullptr) return;
	

	if(gisb->Attenuation.overrideParent)
	{
		// TODO
	}

	if (gisb->VolumeDB.value != 0 || gisb->VolumeDB.randomize)
	{
		FMetaSoundNodeHandle volumeHandle;
		volumeHandle = builder->AddNode(GisbVolumeNode, result);
		if (result == EMetaSoundBuilderResult::Succeeded)
		{
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

            #include <cmath> // Include cmath for std::pow
            
			float linearMax = std::pow(10.0f, gisb->VolumeDB.maxRnd * 0.05f);
			FAudioParameter maxParam = FAudioParameter(TEXT("Max"), linearMax);
			FMetasoundFrontendLiteral maxValue = FMetasoundFrontendLiteral(maxParam);
			builder->SetNodeInputDefault(maxHandle, maxValue, result);

            float linearMin = std::pow(10.0f, gisb->VolumeDB.minRnd * 0.05f);
			FAudioParameter minParam = FAudioParameter(TEXT("Min"), linearMin);
			FMetasoundFrontendLiteral minValue = FMetasoundFrontendLiteral(minParam);
			builder->SetNodeInputDefault(minHandle, minValue, result);

			FAudioParameter randomizeParam = FAudioParameter(TEXT("Randomize"), gisb->VolumeDB.randomize);
			FMetasoundFrontendLiteral randomizeValue = FMetasoundFrontendLiteral(randomizeParam);
			builder->SetNodeInputDefault(randomizeHandle, randomizeValue, result);

			float linearValue = std::pow(10.0f, gisb->VolumeDB.value * 0.05f);
			FAudioParameter valueParam = FAudioParameter(TEXT("Value"), linearValue);
			FMetasoundFrontendLiteral valueValue = FMetasoundFrontendLiteral(valueParam);
			builder->SetNodeInputDefault(valueHandle, valueValue, result);

			firstAudioHandle = builder->FindNodeOutputByName(volumeHandle, TEXT("OutL"), result);
			if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(volumeHandle, TEXT("OutR"), result);
		}
	}

	if(gisb->Pitch.value != 0 || gisb->Pitch.randomize)
	{
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

			#include <cmath> // Include cmath for std::pow

			float linearMax = std::pow(2.0f, gisb->Pitch.maxRnd * 0.00083333f);
			FAudioParameter maxParam = FAudioParameter(TEXT("Min"), linearMax);
			FMetasoundFrontendLiteral maxValue = FMetasoundFrontendLiteral(maxParam);
			builder->SetNodeInputDefault(maxHandle, maxValue, result);

			float linearMin = std::pow(2.0f, gisb->Pitch.minRnd * 0.00083333f);
			FAudioParameter minParam = FAudioParameter(TEXT("Min"), linearMin);
			FMetasoundFrontendLiteral minValue = FMetasoundFrontendLiteral(minParam);
			builder->SetNodeInputDefault(minHandle, minValue, result);

			FAudioParameter randomizeParam = FAudioParameter(TEXT("Randomize"), gisb->Pitch.randomize);
			FMetasoundFrontendLiteral randomizeValue = FMetasoundFrontendLiteral(randomizeParam);
			builder->SetNodeInputDefault(randomizeHandle, randomizeValue, result);

			float linearValue = std::pow(2.0f, gisb->Pitch.value * 0.00083333f);
			FAudioParameter valueParam = FAudioParameter(TEXT("Value"), linearValue);
			FMetasoundFrontendLiteral valueValue = FMetasoundFrontendLiteral(valueParam);
			builder->SetNodeInputDefault(valueHandle, valueValue, result);

			firstAudioHandle = builder->FindNodeOutputByName(pitchHandle, TEXT("OutL"), result);
			if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(pitchHandle, TEXT("OutR"), result);
		}
	}

	if(gisb->Lowpass.value != 0 || gisb->Lowpass.randomize)
	{
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

			// 0 is 22 000
			// 100 is 20

			//21980
			float frequencyMax = gisb->Lowpass.maxRnd * 219.80;
			FAudioParameter maxParam = FAudioParameter(TEXT("Min"), frequencyMax);
			FMetasoundFrontendLiteral maxValue = FMetasoundFrontendLiteral(maxParam);
			builder->SetNodeInputDefault(maxHandle, maxValue, result);

			float frequencyMin = gisb->Lowpass.minRnd * 219.80;
			FAudioParameter minParam = FAudioParameter(TEXT("Min"), frequencyMin);
			FMetasoundFrontendLiteral minValue = FMetasoundFrontendLiteral(minParam);
			builder->SetNodeInputDefault(minHandle, minValue, result);

			FAudioParameter randomizeParam = FAudioParameter(TEXT("Randomize"), gisb->Lowpass.randomize);
			FMetasoundFrontendLiteral randomizeValue = FMetasoundFrontendLiteral(randomizeParam);
			builder->SetNodeInputDefault(randomizeHandle, randomizeValue, result);

			float frequencyValue = (100.0f - FMath::Clamp(gisb->Lowpass.value, 0.0f, 100.0f)) * 219.80 + 20;
			FAudioParameter valueParam = FAudioParameter(TEXT("Value"), frequencyValue);
			FMetasoundFrontendLiteral valueValue = FMetasoundFrontendLiteral(valueParam);
			builder->SetNodeInputDefault(valueHandle, valueValue, result);

			firstAudioHandle = builder->FindNodeOutputByName(lowpassHandle, TEXT("OutL"), result);
			if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(lowpassHandle, TEXT("OutR"), result);
		}
	}

	builder->ConnectNodes(onFinishHandle, *finishHandle, result);

	builder->ConnectNodes(firstAudioHandle, (*audioHandles)[0], result);
	if(audioHandles->Num() > 1 && isStereo)
		builder->ConnectNodes(secondAudioHandle, (*audioHandles)[1], result);
}