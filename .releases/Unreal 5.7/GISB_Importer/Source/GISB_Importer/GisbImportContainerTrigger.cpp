// Copyright Demute SRL. All Rights Reserved.

#include "GisbImportContainerTrigger.h"
#include "GisbContainerTrigger.h"
#include "GisbImportContainerBase.h"

void UGisbImportContainerTrigger::ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path)
{
	// 1. Parse base parameters (Volume, Pitch, Lowpass, Attenuation, etc.)
	Super::ParseJson(JsonObject, Outer, path);

	// 2. Parse trigger-specific parameters
	TriggerAmount = JsonObject->GetIntegerField(TEXT("triggerAmount"));

	int32 TransitionModeInt = JsonObject->GetIntegerField(TEXT("transitionMode"));
	TransitionMode = static_cast<EGisbTriggerTransitionMode>(TransitionModeInt);

	TriggerRate = static_cast<float>(JsonObject->GetNumberField(TEXT("triggerRate")));
	CrossfadeDuration = static_cast<float>(JsonObject->GetNumberField(TEXT("crossfadeDuration")));

	// 3. Recursively parse the triggered child sound
	const TSharedPtr<FJsonObject>* TriggeredSoundJson;
	if (JsonObject->TryGetObjectField(TEXT("TriggeredSound"), TriggeredSoundJson))
	{
		// Use polymorphic factory to create correct import container type
		TriggeredSoundImport = UGisbImportContainerBase::CreateFromJson(*TriggeredSoundJson, Outer, path);
	}

	// Compute cached properties after child is created
	ComputeCachedProperties();
}

UGisbContainerBase* UGisbImportContainerTrigger::ToRuntimeContainer(UObject* Outer)
{
	// 1. Create runtime container instance
	UGisbContainerTrigger* RuntimeContainer = NewObject<UGisbContainerTrigger>(Outer, UGisbContainerTrigger::StaticClass());

	// 2. Copy base parameters (Volume, Pitch, Lowpass, Attenuation, PlaybackProbability)
	AssignBaseVariables(RuntimeContainer);

	// 3. Copy trigger-specific parameters
	RuntimeContainer->TriggerAmount = TriggerAmount;
	RuntimeContainer->TransitionMode = TransitionMode;
	RuntimeContainer->TriggerRate = TriggerRate;
	RuntimeContainer->CrossfadeDuration = CrossfadeDuration;

	// 4. Recursively convert child import container to runtime container
	if (TriggeredSoundImport)
	{
		RuntimeContainer->TriggeredSound = TriggeredSoundImport->ToRuntimeContainer(RuntimeContainer);
	}

	return RuntimeContainer;
}

void UGisbImportContainerTrigger::ComputeCachedProperties()
{
	// Single child container: inherit properties from triggered child
	if (TriggeredSoundImport)
	{
		bIsStereo = TriggeredSoundImport->bIsStereo;
		bIsLooping = TriggeredSoundImport->bIsLooping;
	}
	else
	{
		bIsStereo = false;
		bIsLooping = false;
	}
	
	//Checking looping capabilities of trigger container
	bIsLooping = 
		bIsLooping || 
		TransitionMode == EGisbTriggerTransitionMode::Crossfade ||
		(TransitionMode == EGisbTriggerTransitionMode::TriggerRate && TriggerAmount == -1);
	
}
