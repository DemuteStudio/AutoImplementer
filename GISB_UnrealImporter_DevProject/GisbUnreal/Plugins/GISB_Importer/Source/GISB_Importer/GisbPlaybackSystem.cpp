// Fill out your copyright notice in the Description page of Project Settings.


#include "GisbPlaybackSystem.h"

#include "Kismet/GameplayStatics.h"

UAudioComponent* UGisbPlaybackSystem::PlaySoundEvent(UObject* WorldContextObject, UGisbSoundBankDataAsset* SoundBank, const TMap<FName, FString>& Parameters)
{
	if (!WorldContextObject || !SoundBank)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid parameters for PlaySoundEvent"));
		return nullptr;
	}

	UGisbContainerBase* RootContainer = SoundBank->RootContainer;
	if (!RootContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundBank has no RootContainer"));
		return nullptr;
	}

	// Resolve the actual sound to play based on container logic
	RootContainer->Play(WorldContextObject, Parameters);
	return nullptr;
}
