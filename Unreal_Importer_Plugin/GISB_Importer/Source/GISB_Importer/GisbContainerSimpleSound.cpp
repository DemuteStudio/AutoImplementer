// Fill out your copyright notice in the Description page of Project Settings.


#include "GisbContainerSimpleSound.h"

#include "Kismet/GameplayStatics.h"

void UGisbContainerSimpleSound::Play(UObject* WorldContext, const TMap<FName, FString>& Parameters)
{
	if (!Sound) return;

	UGameplayStatics::PlaySound2D(WorldContext, Sound);
}
