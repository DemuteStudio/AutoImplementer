// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbImportContainerBase.h"
#include "Dom/JsonObject.h"

#include "GisbImportContainerRandom.h"
#include "GisbImportContainerBlend.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerSwitch.h"

UGisbImportContainerBase* UGisbImportContainerBase::CreateFromJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path)
{
	FString Type = JsonObject->GetStringField("$type");

	UGisbImportContainerBase* Container = nullptr;

	if (Type == "GISB.Runtime.GISB_SingleSound, Demute.GISB")
	{
		Container = NewObject<UGisbImportContainerSimpleSound>(Outer);
	}
	else if (Type == "GISB.Runtime.GISB_RandomSound, Demute.GISB")
	{
		Container = NewObject<UGisbImportContainerRandom>(Outer);
	}
	else if (Type == "GISB.Runtime.GISB_SwitchSound, Demute.GISB")
	{
		Container = NewObject<UGisbImportContainerSwitch>(Outer);
	}
	else if (Type == "GISB.Runtime.GISB_BlendSound, Demute.GISB")
	{
		Container = NewObject<UGisbImportContainerBlend>(Outer);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown container type: %s"), *Type);
	}

	if (Container)
	{
		Container->Type = Type;
		Container->ParseJson(JsonObject, Outer, path);
	}

	return Container;
}

void UGisbImportContainerBase::ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path)
{
	TSharedPtr<FJsonObject> JsonAttenuation = JsonObject->GetObjectField("attenuation");
	TSharedPtr<FJsonObject> JsonAttenuationValue = JsonAttenuation->GetObjectField("value");
	Attenuation = FGisbAttenuation{
		JsonAttenuation->GetBoolField("overrideParent"),
		JsonAttenuationValue->GetBoolField("active"),
		(float)JsonAttenuationValue->GetNumberField("minDistance"),
		(float)JsonAttenuationValue->GetNumberField("maxDistance"),
		//TBD curve
		(float)JsonAttenuationValue->GetNumberField("volumeAtMaxDistance"),
		(float)JsonAttenuationValue->GetNumberField("spreadAtMinDistance"),
		(float)JsonAttenuationValue->GetNumberField("spreadAtMaxDistance"),
		(float)JsonAttenuationValue->GetNumberField("lowPassAtMinDistance"),
		(float)JsonAttenuationValue->GetNumberField("lowPassAtMaxDistance")
	};

	TSharedPtr<FJsonObject> JsonVolume = JsonObject->GetObjectField("volumeDB");
	VolumeDB = FGisbVolume{
	   (float)JsonVolume->GetNumberField("baseValue"),
	   JsonVolume->GetBoolField("randomize"),
	   (float)JsonVolume->GetNumberField("minModifier"),
	   (float)JsonVolume->GetNumberField("maxModifier")
	};

	TSharedPtr<FJsonObject> JsonPitch = JsonObject->GetObjectField("pitchCents");
	Pitch = FGisbPitch{
	   (float)JsonPitch->GetNumberField("baseValue"),
	   JsonPitch->GetBoolField("randomize"),
	   (float)JsonPitch->GetNumberField("minModifier"),
	   (float)JsonPitch->GetNumberField("maxModifier")
	};

	TSharedPtr<FJsonObject> JsonLowpass = JsonObject->GetObjectField("lowpass");
    Lowpass = FGisbLowPass{
       (float)JsonLowpass->GetNumberField("baseValue"),
       JsonLowpass->GetBoolField("randomize"),
       (float)JsonLowpass->GetNumberField("minModifier"),
       (float)JsonLowpass->GetNumberField("maxModifier")
    };
	
	PlaybackProbabilityPercent = JsonObject->GetNumberField("playbackProbabilityPercent");
}

void UGisbImportContainerBase::AssignBaseVariables(UGisbContainerBase* Container)
{
	Container->Attenuation = Attenuation;
	Container->VolumeDB = VolumeDB;
	Container->Pitch = Pitch;
	Container->Lowpass = Lowpass;
	Container->PlaybackProbabilityPercent = PlaybackProbabilityPercent;
}
