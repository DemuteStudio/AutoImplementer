// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbImportContainerSwitch.h"

#include "GisbContainerSwitch.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "GisbImportContainerSimpleSound.h"

void UGisbImportContainerSwitch::ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path)
{
	Super::ParseJson(JsonObject, Outer, path);
	ParameterID = FName(*JsonObject->GetStringField(TEXT("ParameterName")));
	DefaultParameterValue = FName(*JsonObject->GetStringField(TEXT("DefaultValue")));
	const TArray<TSharedPtr<FJsonValue>>* SoundArrayJson;
	if (JsonObject->TryGetArrayField(TEXT("SwitchCases"), SoundArrayJson))
	{
		for (const auto& SoundEntry : *SoundArrayJson)
		{
			TSharedPtr<FJsonObject> SoundObj = SoundEntry->AsObject();
			FString ParameterVal = SoundObj->GetStringField(TEXT("parameterValue"));

			const TSharedPtr<FJsonObject>* AudioObjectJson;
			SoundObj->TryGetObjectField(TEXT("audioObject"),AudioObjectJson);
			UGisbImportContainerBase* Container = CreateFromJson(*AudioObjectJson, Outer, path);

			if (Container)
			{
				SoundDictionary.Add(ParameterVal, Container);
			}
		}
	}

	// Compute cached properties after all children are created
	ComputeCachedProperties();
}

UGisbContainerBase* UGisbImportContainerSwitch::ToRuntimeContainer(UObject* Outer)
{
	UGisbContainerSwitch* RuntimeContainer = NewObject<UGisbContainerSwitch>(Outer, UGisbContainerSwitch::StaticClass(), NAME_None, RF_Public | RF_Transactional | RF_Standalone);
	AssignBaseVariables(RuntimeContainer);
	RuntimeContainer->ParameterName = ParameterID;
	RuntimeContainer->ParameterValue = DefaultParameterValue;
	for (const auto& Elem : SoundDictionary)
	{
		if (Elem.Value)
		{
			UGisbContainerBase* ChildRuntime = Elem.Value->ToRuntimeContainer(RuntimeContainer);
			RuntimeContainer->SwitchMap.Add(Elem.Key, ChildRuntime);
			RuntimeContainer->MarkPackageDirty();
		}
	}
	return RuntimeContainer;
}

void UGisbImportContainerSwitch::ComputeCachedProperties()
{
	// Composite node: check if ANY child in dictionary is stereo or looping
	bIsStereo = false;
	bIsLooping = false;

	for (const auto& Pair : SoundDictionary)
	{
		if (Pair.Value)
		{
			if (Pair.Value->bIsStereo) bIsStereo = true;
			if (Pair.Value->bIsLooping) bIsLooping = true;

			// Early exit if both are true
			if (bIsStereo && bIsLooping) break;
		}
	}
}