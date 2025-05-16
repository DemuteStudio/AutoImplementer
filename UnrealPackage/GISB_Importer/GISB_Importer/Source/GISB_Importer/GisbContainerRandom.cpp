// Fill out your copyright notice in the Description page of Project Settings.


#include "GisbContainerRandom.h"

void UGisbContainerRandom::Play(UObject* WorldContext, const TMap<FName, FString>& Parameters)
{
	if (ChildContainers.Num() == 0) return;

	// Pick weighted random sound
	int32 Index = FMath::RandRange(0, ChildContainers.Num() - 1);

	if (ChildContainers[Index])
	{
		ChildContainers[Index]->Play(WorldContext, Parameters);
	}
}