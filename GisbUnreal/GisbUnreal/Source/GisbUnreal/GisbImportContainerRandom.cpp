// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbImportContainerRandom.h"

#include "GisbContainerRandom.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "GisbImportContainerSimpleSound.h"

void UGisbImportContainerRandom::ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path)
{
	AvoidLastPlayed = FCString::Atoi(*JsonObject->GetStringField("avoidLastPlayed"));
	const TArray<TSharedPtr<FJsonValue>>* SoundArrayJson;
	if (JsonObject->TryGetArrayField("RandomPlaylist", SoundArrayJson))
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
}

UGisbContainerBase* UGisbImportContainerRandom::ToRuntimeContainer(UObject* Outer)
{
	UGisbContainerRandom* RuntimeContainer = NewObject<UGisbContainerRandom>(Outer, UGisbContainerRandom::StaticClass(), NAME_None, RF_Public | RF_Transactional | RF_Standalone);
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