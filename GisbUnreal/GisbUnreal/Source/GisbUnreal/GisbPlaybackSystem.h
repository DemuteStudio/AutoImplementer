// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "GisbSoundBankDataAsset.h"
#include "GisbPlaybackSystem.generated.h"

UCLASS()
class GISBUNREAL_API UGisbPlaybackSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** Plays a sound event from a data asset and returns the audio component playing the sound */
	UFUNCTION(BlueprintCallable, Category = "Gisb Audio")
	static UAudioComponent* PlaySoundEvent(UObject* WorldContextObject, UGisbSoundBankDataAsset* SoundBank, const TMap<FName, FString>& Parameters);
};

