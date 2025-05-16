// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbSoundBankParser.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "GisbImportContainerSwitch.h"

void ParseSoundBank(const FString& JsonString)
{
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject;

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		const TSharedPtr<FJsonObject>* Events;
		if (JsonObject->TryGetObjectField("soundBankEvents", Events))
		{
			for (const auto& Event : (*Events)->Values)
			{
				if (Event.Value->IsNull()) continue;

				TSharedPtr<FJsonObject> EventObj = Event.Value->AsObject();
				const TSharedPtr<FJsonObject>* RootContainerJson;
				if (EventObj->TryGetObjectField("rootSoundContainer", RootContainerJson))
				{

				}
			}
		}
	}
}
