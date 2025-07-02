// Fill out your copyright notice in the Description page of Project Settings.


#include "GisbContainerSwitch.h"

void UGisbContainerSwitch::Play(UObject* WorldContext, const TMap<FName, FString>& Parameters)
{
	if (!Parameters.Contains(ParameterName)) return;

	FString Value = Parameters[ParameterName];
	if (TObjectPtr<UGisbContainerBase>* Found = SwitchMap.Find(Value))
	{
		if (*Found)
		{
			(*Found)->Play(WorldContext, Parameters);
		}
	}
}