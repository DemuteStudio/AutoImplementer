// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbImportContainerRandom.h"

#include "GisbContainerRandom.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "GisbImportContainerSimpleSound.h"

void UGisbImportContainerRandom::ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path)
{
	Super::ParseJson(JsonObject, Outer, path);
	AvoidLastPlayed = FCString::Atoi(*JsonObject->GetStringField(TEXT("avoidLastPlayed")));
	const TArray<TSharedPtr<FJsonValue>>* SoundArrayJson;
	if (JsonObject->TryGetArrayField(TEXT("RandomPlaylist"), SoundArrayJson))
	{
		for (const auto& SoundEntry : *SoundArrayJson)
		{
			TSharedPtr<FJsonObject> SoundObj = SoundEntry->AsObject();
			UGisbImportContainerBase* NewContainer = CreateFromJson(SoundObj, Outer, path);
			if (NewContainer)
			{
				SoundArray.Add(NewContainer);
			}
		}
	}

	// Compute cached properties after all children are created
	ComputeCachedProperties();
}

UGisbContainerBase* UGisbImportContainerRandom::ToRuntimeContainer(UObject* Outer)
{
	UGisbContainerRandom* RuntimeContainer = NewObject<UGisbContainerRandom>(Outer, UGisbContainerRandom::StaticClass(), NAME_None, RF_Public | RF_Transactional | RF_Standalone);
	AssignBaseVariables(RuntimeContainer);
	for (int32 i = 0; i < SoundArray.Num(); ++i)
	{
		if (SoundArray[i])
		{
			UGisbContainerBase* ChildRuntime = SoundArray[i]->ToRuntimeContainer(RuntimeContainer);
			RuntimeContainer->ChildContainers.Add(ChildRuntime);
			RuntimeContainer->MarkPackageDirty();
		}
	}

	return RuntimeContainer;
}

void UGisbImportContainerRandom::ComputeCachedProperties()
{
	// Composite node: check if ANY child is stereo or looping
	bIsStereo = false;
	bIsLooping = false;

	for (UGisbImportContainerBase* Child : SoundArray)
	{
		if (Child)
		{
			// Children already have their properties computed
			if (Child->bIsStereo) bIsStereo = true;
			if (Child->bIsLooping) bIsLooping = true;

			// Early exit if both are true
			if (bIsStereo && bIsLooping) break;
		}
	}
}