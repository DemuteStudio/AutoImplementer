// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbImportContainerBlend.h"

#include "GisbContainerBlend.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "GisbImportContainerSimpleSound.h"

void UGisbImportContainerBlend::ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path)
{
	Super::ParseJson(JsonObject, Outer, path);
	const TArray<TSharedPtr<FJsonValue>>* SoundArrayJson;
	if (JsonObject->TryGetArrayField(TEXT("BlendPlaylist"), SoundArrayJson))
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

UGisbContainerBase* UGisbImportContainerBlend::ToRuntimeContainer(UObject* Outer)
{
	UGisbContainerBlend* RuntimeContainer = NewObject<UGisbContainerBlend>(Outer, UGisbContainerBlend::StaticClass(), NAME_None, RF_Public | RF_Transactional | RF_Standalone);
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