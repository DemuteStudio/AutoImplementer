// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbImportContainerBase.h"
#include "Dom/JsonObject.h"
#include "GisbImportContainerRandom.h"
#include "GisbImportContainerBlend.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerSwitch.h"

UGisbImportContainerBase* UGisbImportContainerBase::CreateFromJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path)
{
	FString Type = JsonObject->GetStringField(TEXT("$type"));

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
	TSharedPtr<FJsonObject> JsonAttenuation = JsonObject->GetObjectField(TEXT("attenuation"));
	TSharedPtr<FJsonObject> JsonAttenuationValue = JsonAttenuation->GetObjectField(TEXT("value"));

	FRuntimeFloatCurve* AttenuationCurve = new FRuntimeFloatCurve();
	FRichCurve* curve = &AttenuationCurve->EditorCurveData;
	TArray<FRichCurveKey> tempKeys;
	int curvePreset = JsonAttenuationValue->GetIntegerField(TEXT("preset"));

	switch (curvePreset)
	{
		default:
		case 0: //Linear preset
		{
			curve->AddKey(0, 1);
			curve->AddKey(1, 0);
		}
		break;

		case 1: //Logarithmic preset
		{
			FKeyHandle LogH1, LogH2;

			LogH1 = curve->AddKey(0, 1);
			curve->SetKeyTangentMode(LogH1, ERichCurveTangentMode::RCTM_Break);
			curve->SetKeyInterpMode(LogH1, ERichCurveInterpMode::RCIM_Cubic);

			LogH2 = curve->AddKey(1, 0);
			curve->SetKeyTangentMode(LogH2, ERichCurveTangentMode::RCTM_Break);
			curve->SetKeyInterpMode(LogH2, ERichCurveInterpMode::RCIM_Cubic);

			tempKeys = curve->Keys;

			tempKeys[0].ArriveTangent = 0.0f;
			tempKeys[0].LeaveTangent = -2.302f;
			tempKeys[1].ArriveTangent = -0.105f;
			tempKeys[1].LeaveTangent = 0.0f;

			curve->SetKeys(tempKeys);
		}
		break;

		case 2: // Inverse preset
		{
			FKeyHandle InvH1, InvH2;

			InvH1 = curve->AddKey(0, 1);
			curve->SetKeyTangentMode(InvH1, ERichCurveTangentMode::RCTM_Break);
			curve->SetKeyInterpMode(InvH1, ERichCurveInterpMode::RCIM_Cubic);

			InvH2 = curve->AddKey(1, 0);
			curve->SetKeyTangentMode(InvH2, ERichCurveTangentMode::RCTM_Break);
			curve->SetKeyInterpMode(InvH2, ERichCurveInterpMode::RCIM_Cubic);

			tempKeys = curve->Keys;

			tempKeys[0].ArriveTangent = 0.0f;
			tempKeys[0].LeaveTangent = -3.0f;
			tempKeys[1].ArriveTangent = 0.0f;
			tempKeys[1].LeaveTangent = 0.0f;

			curve->SetKeys(tempKeys);
		}
		break;

		case 3: // No preset (Custom)
		{
			TSharedPtr<FJsonObject> JsonCurveValue = JsonAttenuationValue->GetObjectField(TEXT("curve"));
			TArray<TSharedPtr<FJsonValue>> JsonKeys = JsonCurveValue->GetArrayField(TEXT("keys"));

			int length = JsonCurveValue->GetIntegerField(TEXT("length"));
			int preWrapMode = JsonCurveValue->GetIntegerField(TEXT("preWrapMode"));
			int postWrapMode = JsonCurveValue->GetIntegerField(TEXT("postWrapMode"));
			
			for(int i = 0; i<length; i++)
			{
				TSharedPtr<FJsonObject> JsonKey = JsonKeys[i]->AsObject();
				float time = (float)JsonKey->GetNumberField(TEXT("time"));
				float value = (float)JsonKey->GetNumberField(TEXT("value"));
				auto handle = curve->AddKey(time, value);
				ERichCurveTangentMode RCTM = (ERichCurveTangentMode)JsonKey->GetIntegerField(TEXT("tangentMode")); // TO DO : Check if Unity's tangent mode equals UE's one
				ERichCurveInterpMode RCIM = ERichCurveInterpMode::RCIM_Cubic; // Not right, just placeholder
				//(ERichCurveInterpMode)JsonKey->GetIntegerField("weightedMode"); // Not right, just placeholder
				curve->SetKeyTangentMode(handle, ERichCurveTangentMode::RCTM_Break);
				curve->SetKeyInterpMode(handle, ERichCurveInterpMode::RCIM_Cubic);
			}

			tempKeys = curve->Keys;

			for (int i = 0; i < length; i++)
			{

				TSharedPtr<FJsonObject> JsonKey = JsonKeys[i]->AsObject();

				float inTngnt = (float)JsonKey->GetNumberField(TEXT("inTangent"));
				float outTngnt = (float)JsonKey->GetNumberField(TEXT("outTangent"));
				float inWeight = (float)JsonKey->GetNumberField(TEXT("inWeight"));
				float outWeight = (float)JsonKey->GetNumberField(TEXT("outWeight"));
				
				tempKeys[i].ArriveTangent = inTngnt;
				tempKeys[i].LeaveTangent = outTngnt;
				tempKeys[i].ArriveTangentWeight = inWeight;
				tempKeys[i].LeaveTangentWeight = outWeight;
			}

			curve->SetKeys(tempKeys);
		}
		break;
	}
	

	Attenuation = FGisbAttenuation{
		JsonAttenuation->GetBoolField(TEXT("overrideParent")),
		JsonAttenuationValue->GetBoolField(TEXT("active")),
		(float)JsonAttenuationValue->GetNumberField(TEXT("minDistance")),
		(float)JsonAttenuationValue->GetNumberField(TEXT("maxDistance")),
		*AttenuationCurve,
		(float)JsonAttenuationValue->GetNumberField(TEXT("volumeAtMaxDistance")),
		(float)JsonAttenuationValue->GetNumberField(TEXT("spreadAtMinDistance")),
		(float)JsonAttenuationValue->GetNumberField(TEXT("spreadAtMaxDistance")),
		(float)JsonAttenuationValue->GetNumberField(TEXT("lowPassAtMinDistance")),
		(float)JsonAttenuationValue->GetNumberField(TEXT("lowPassAtMaxDistance"))
	};

	TSharedPtr<FJsonObject> JsonVolume = JsonObject->GetObjectField(TEXT("volumeDB"));
	VolumeDB = FGisbVolume{
	   (float)JsonVolume->GetNumberField(TEXT("baseValue")),
	   JsonVolume->GetBoolField(TEXT("randomize")),
	   (float)JsonVolume->GetNumberField(TEXT("minModifier")),
	   (float)JsonVolume->GetNumberField(TEXT("maxModifier"))
	};

	TSharedPtr<FJsonObject> JsonPitch = JsonObject->GetObjectField(TEXT("pitchCents"));
	Pitch = FGisbPitch{
	   (float)JsonPitch->GetNumberField(TEXT("baseValue")),
	   JsonPitch->GetBoolField(TEXT("randomize")),
	   (float)JsonPitch->GetNumberField(TEXT("minModifier")),
	   (float)JsonPitch->GetNumberField(TEXT("maxModifier"))
	};

	TSharedPtr<FJsonObject> JsonLowpass = JsonObject->GetObjectField(TEXT("lowpass"));
    Lowpass = FGisbLowPass{
       (float)JsonLowpass->GetNumberField(TEXT("baseValue")),
       JsonLowpass->GetBoolField(TEXT("randomize")),
       (float)JsonLowpass->GetNumberField(TEXT("minModifier")),
       (float)JsonLowpass->GetNumberField(TEXT("maxModifier"))
    };
	
	PlaybackProbabilityPercent = JsonObject->GetNumberField(TEXT("playbackProbabilityPercent"));
}

void UGisbImportContainerBase::AssignBaseVariables(UGisbContainerBase* Container) const
{
	Container->Attenuation = Attenuation;
	Container->VolumeDB = VolumeDB;
	Container->Pitch = Pitch;
	Container->Lowpass = Lowpass;
	Container->PlaybackProbabilityPercent = PlaybackProbabilityPercent;
}