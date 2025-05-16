// Fill out your copyright notice in the Description page of Project Settings.


#include "GISB_MetasoundBuilder.h"
#include "MetasoundSource.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundBuilderSubsystem.h"
#include "MetasoundBuilderBase.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "MetasoundEditorSubsystem.h"

FMetasoundFrontendClassName* UGISB_MetasoundBuilder::WavePlayerMonoNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundBuilder::WavePlayerStereoNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundBuilder::GisbRandomNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundBuilder::GisbSwitchNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundBuilder::GisbProbabilityNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundBuilder::GisbVolumeNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundBuilder::GisbPitchNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundBuilder::GisbLowpassNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundBuilder::GisbAttenuationNode = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundBuilder::MonoMixerNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundBuilder::StereoMixerNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundBuilder::TriggerAnyNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundBuilder::TriggerAccumulateNodes = nullptr;

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
	FName none = TEXT("None");

	WavePlayerMonoNode = new FMetasoundFrontendClassName(ue, wave, TEXT("Mono"));
	WavePlayerStereoNode = new FMetasoundFrontendClassName(ue, wave, TEXT("Stereo"));
	GisbAttenuationNode = new FMetasoundFrontendClassName(none, TEXT("D3CBA5154B55E34A2AE5A0BF21DF0423"));
	GisbProbabilityNode = new FMetasoundFrontendClassName(ue, TEXT("Trigger Filter"));
	GisbLowpassNode = new FMetasoundFrontendClassName(none, TEXT("C3374D90426FC65520BE6CAC665B0A65"));
	GisbPitchNode = new FMetasoundFrontendClassName(none, TEXT("22EDDDC74271DA7F01C55EBB71705FED"));
	GisbRandomNode = new FMetasoundFrontendClassName(none, TEXT("F6D8C5454BB6D02AD68FC49389C4B516"));
	GisbVolumeNode = new FMetasoundFrontendClassName(none, TEXT("DE4C817A42F6754CAFFE3D80100B3A5F"));
	GisbSwitchNode = new FMetasoundFrontendClassName(none, TEXT("5C1E1A1249407BA16A3A4A8AAF5CBD7F"));

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
		if(simplesound->SoundWave->NumChannels > 1) shouldStereo = true;
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
	bool isStereo = false;

	if (gisb->PlaybackProbabilityPercent < 99.99f)
	{

		FMetaSoundNodeHandle probabilityHandle;
		probabilityHandle = builder->AddNodeByClassName(*GisbProbabilityNode, result);
		if (result == EMetaSoundBuilderResult::Succeeded)
		{
			FMetaSoundBuilderNodeInputHandle floatHandle = builder->FindNodeInputByName(probabilityHandle, TEXT("Probability"), result);
			FMetaSoundBuilderNodeInputHandle triggerHandle = builder->FindNodeInputByName(probabilityHandle, TEXT("Wave Asset"), result);

			FAudioParameter floatParam = FAudioParameter(TEXT("Wave Asset"), gisb->PlaybackProbabilityPercent);
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
				FMetaSoundBuilderNodeInputHandle succesHandle = builder->FindNodeInputByName(anyHandle, TEXT("In 0"), result);
				FMetaSoundBuilderNodeInputHandle failHandle = builder->FindNodeInputByName(anyHandle, TEXT("In 1"), result);

				builder->ConnectNodes(tailsHandle, failHandle, result);
				finishHandle = &succesHandle;
			}
		}
	}

	UGisbImportContainerSimpleSound* simplesound = Cast<UGisbImportContainerSimpleSound>(gisb);
	if (simplesound != nullptr)
	{
		isStereo = simplesound->SoundWave->NumChannels > 1;
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
			
			if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(handle, TEXT("Out Right"), result);
		}
	}

	UGisbImportContainerBlend* blend = Cast<UGisbImportContainerBlend>(gisb);
	if (blend != nullptr)
	{
		for (int i = 0; i < blend->SoundArray.Num(); i++)
		{
			UGisbImportContainerBase* newGisb = blend->SoundArray[i];
			ConnectContainerToGraph(builder, newGisb, executionHandle, finishHandle, audioHandles);
		}
		// Create blend node at the beginning and at the end
	}

	UGisbImportContainerRandom* random = Cast<UGisbImportContainerRandom>(gisb);
	if (random != nullptr)
	{
		for (int i = 0; i < random->SoundArray.Num(); i++)
		{
			UGisbImportContainerBase* newGisb = random->SoundArray[i];
			ConnectContainerToGraph(builder, newGisb, executionHandle, finishHandle, audioHandles);
		}
		// Create random node at the beginning and at the end
		
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
		volumeHandle = builder->AddNodeByClassName(*GisbVolumeNode, result);
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

			firstAudioHandle = builder->FindNodeOutputByName(volumeHandle, TEXT("Heads"), result);
			if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(volumeHandle, TEXT("Tails"), result);
		}
	}

	if(gisb->Pitch.value != 0 || gisb->Pitch.randomize)
	{
		FMetaSoundNodeHandle pitchHandle;
		pitchHandle = builder->AddNodeByClassName(*GisbPitchNode, result);
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

			firstAudioHandle = builder->FindNodeOutputByName(pitchHandle, TEXT("Heads"), result);
			if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(pitchHandle, TEXT("Tails"), result);
		}
	}

	if(gisb->Lowpass.value != 0 || gisb->Lowpass.randomize)
	{
		FMetaSoundNodeHandle lowpassHandle;
		lowpassHandle = builder->AddNodeByClassName(*GisbLowpassNode, result);
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
			float frequencyMax = (100.0f - gisb->Lowpass.maxRnd) * 219.80;
			FAudioParameter maxParam = FAudioParameter(TEXT("Min"), frequencyMax);
			FMetasoundFrontendLiteral maxValue = FMetasoundFrontendLiteral(maxParam);
			builder->SetNodeInputDefault(maxHandle, maxValue, result);

			float frequencyMin = (100.0f - gisb->Lowpass.minRnd) * 219.80;
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

			firstAudioHandle = builder->FindNodeOutputByName(lowpassHandle, TEXT("Heads"), result);
			if (isStereo) secondAudioHandle = builder->FindNodeOutputByName(lowpassHandle, TEXT("Tails"), result);
		}
	}

	builder->ConnectNodes(onFinishHandle, *finishHandle, result);

	builder->ConnectNodes(firstAudioHandle, (*audioHandles)[0], result);
	if(audioHandles->Num() > 1 && isStereo)
		builder->ConnectNodes(secondAudioHandle, (*audioHandles)[1], result);
}