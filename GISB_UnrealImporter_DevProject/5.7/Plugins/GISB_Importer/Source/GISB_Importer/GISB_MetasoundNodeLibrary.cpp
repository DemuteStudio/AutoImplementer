// Copyright Demute SRL. All Rights Reserved.

#include "GISB_MetasoundNodeLibrary.h"

#include "MetasoundExampleNodeConfiguration.h"
#include "MetasoundSource.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"

// Static member variable initializations
FMetasoundFrontendClassName* UGISB_MetasoundNodeLibrary::WavePlayerMonoNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundNodeLibrary::WavePlayerStereoNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundNodeLibrary::ProbabilityNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundNodeLibrary::TriggerRepeatNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundNodeLibrary::TriggerCounterNode = nullptr;

TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundNodeLibrary::MonoMixerNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundNodeLibrary::StereoMixerNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundNodeLibrary::TriggerAnyNodes = nullptr;
TArray<FMetasoundFrontendClassName*>* UGISB_MetasoundNodeLibrary::TriggerAccumulateNodes = nullptr;

TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundNodeLibrary::GisbRandomNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundNodeLibrary::GisbVolumeNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundNodeLibrary::GisbPitchNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundNodeLibrary::GisbLowpassNode = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundNodeLibrary::GisbTimedCrossfadeMono = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundNodeLibrary::GisbTimedCrossfadeStereo = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundNodeLibrary::GisbCrossfadeTimer = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundNodeLibrary::GisbTriggerStopper = nullptr;
TScriptInterface<IMetaSoundDocumentInterface> UGISB_MetasoundNodeLibrary::GisbTimeRemaining = nullptr;

FMetasoundFrontendClassName* UGISB_MetasoundNodeLibrary::GisbSwitchNode = nullptr;
FMetasoundFrontendClassName* UGISB_MetasoundNodeLibrary::GisbAttenuationNode = nullptr;

void UGISB_MetasoundNodeLibrary::SetupNodes()
{
	FName ue = TEXT("UE");
	FName wave = TEXT("Wave Player");
	FName config = TEXT("Experimental");

	WavePlayerMonoNode = new FMetasoundFrontendClassName(ue, wave, TEXT("Mono"));

	WavePlayerStereoNode = new FMetasoundFrontendClassName(ue, wave, TEXT("Stereo"));
	
	ProbabilityNode = new FMetasoundFrontendClassName(ue, TEXT("Trigger Filter"));

	UMetaSoundPatch* lowpassPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomLowpass.GISB_RandomLowpass")));
	GisbLowpassNode = TScriptInterface<IMetaSoundDocumentInterface>(lowpassPatch);

	UMetaSoundPatch* pitchPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomPitch.GISB_RandomPitch")));
	GisbPitchNode = TScriptInterface<IMetaSoundDocumentInterface>(pitchPatch);

	UMetaSoundPatch* randomPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomPlayer.GISB_RandomPlayer")));
	GisbRandomNode = TScriptInterface<IMetaSoundDocumentInterface>(randomPatch);

	UMetaSoundPatch* volumePatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_RandomVolume.GISB_RandomVolume")));
	GisbVolumeNode = TScriptInterface<IMetaSoundDocumentInterface>(volumePatch);
	
	UMetaSoundPatch* timedCFMono = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_TimedCrossfade_Mono.GISB_TimedCrossfade_Mono")));
	GisbTimedCrossfadeMono = TScriptInterface<IMetaSoundDocumentInterface>(timedCFMono);
	
	UMetaSoundPatch* timedCFStereo = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_TimedCrossfade_Stereo.GISB_TimedCrossfade_Stereo")));
	GisbTimedCrossfadeStereo = TScriptInterface<IMetaSoundDocumentInterface>(timedCFStereo);
	
	UMetaSoundPatch* CFTimer = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_CrossfadeTrigger.GISB_CrossfadeTrigger")));
	GisbCrossfadeTimer = TScriptInterface<IMetaSoundDocumentInterface>(CFTimer);
	
	UMetaSoundPatch* TrigStopPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_TriggerStopper.GISB_TriggerStopper")));
	GisbTriggerStopper = TScriptInterface<IMetaSoundDocumentInterface>(TrigStopPatch);
	
	UMetaSoundPatch* TimeRemainingPatch = Cast<UMetaSoundPatch>(StaticLoadObject(UMetaSoundPatch::StaticClass(), nullptr, TEXT("/GISB_Importer/GISB_TimeRemaining.GISB_TimeRemaining")));
	GisbTimeRemaining = TScriptInterface<IMetaSoundDocumentInterface>(TimeRemainingPatch);
	
	TriggerRepeatNode = new FMetasoundFrontendClassName(ue, TEXT("TriggerRepeat"));
	TriggerCounterNode = new FMetasoundFrontendClassName(ue, TEXT("Trigger Counter"));
	
	GisbAttenuationNode = new FMetasoundFrontendClassName(config, TEXT("AttenuationOperator"));
	GisbSwitchNode = new FMetasoundFrontendClassName(config, TEXT("StringSwitchOperator"));

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
