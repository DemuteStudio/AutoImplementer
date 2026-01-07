// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbImportContainerBase.h"
#include "Dom/JsonObject.h"
#include "GisbImportContainerRandom.h"
#include "GisbImportContainerBlend.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerSwitch.h"
#include "GisbImportContainerTrigger.h"
#include "AssetRegistry/AssetRegistryModule.h"

UGisbImportContainerBase* UGisbImportContainerBase::CreateFromJson(const TSharedPtr<FJsonObject>& JsonObject,
                                                                   UObject* Outer, const FString& path)
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
	else if (Type == "GISB.Runtime.GISB_TriggerSound, Demute.GISB")
	{
		Container = NewObject<UGisbImportContainerTrigger>(Outer);
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
	TSharedPtr<FJsonObject> JsonAttenuationCurve = JsonAttenuationValue->GetObjectField(TEXT("attenuationCurve"));

	//Save attenuation asset
	FString AssetName = JsonAttenuationValue->GetStringField(TEXT("name"));
	FString AssetPath = FPaths::Combine("/Game/SoundBanks/", AssetName);
	if (FPackageName::IsValidLongPackageName(AssetPath))
	{
		UPackage* Package = CreatePackage(*AssetPath);
		if (Package)
		{
			USoundAttenuation* SoundAttenuation = NewObject<USoundAttenuation>(
				Package, *AssetName, RF_Public | RF_Standalone);
			SoundAttenuation->Modify();
			SoundAttenuation->Attenuation.bAttenuate = JsonAttenuationValue->GetBoolField(TEXT("active"));
			SoundAttenuation->Attenuation.bSpatialize = JsonAttenuationValue->GetBoolField(TEXT("active"));
			SoundAttenuation->Attenuation.AttenuationShape = EAttenuationShape::Sphere;
			SoundAttenuation->Attenuation.FalloffDistance = (float)JsonAttenuationValue->GetNumberField(
				TEXT("maxDistance"));
			SoundAttenuation->Attenuation.AttenuationShapeExtents = FVector(
				(float)JsonAttenuationValue->GetNumberField(TEXT("minDistance")),
				(float)JsonAttenuationValue->GetNumberField(TEXT("minDistance")),
				(float)JsonAttenuationValue->GetNumberField(TEXT("minDistance"))
			);
			SoundAttenuation->Attenuation.bAttenuateWithLPF = true;
			SoundAttenuation->Attenuation.LPFRadiusMin = (float)JsonAttenuationValue->GetNumberField(TEXT("minDistance"));
			SoundAttenuation->Attenuation.LPFRadiusMax = (float)JsonAttenuationValue->GetNumberField(TEXT("maxDistance"));
			SoundAttenuation->Attenuation.LPFFrequencyAtMin = 20000.0f - (200.0f * (float)JsonAttenuationValue->GetNumberField(TEXT("lowPassAtMinDistance")));
			SoundAttenuation->Attenuation.LPFFrequencyAtMax = 20000.0f - (200.0f * (float)JsonAttenuationValue->GetNumberField(TEXT("lowPassAtMaxDistance")));

			FRichCurve* curve = &SoundAttenuation->Attenuation.CustomAttenuationCurve.EditorCurveData;
			FRichCurve* lowPassCurve = &SoundAttenuation->Attenuation.CustomLowpassAirAbsorptionCurve.EditorCurveData;
			FRichCurve* highPassCurve = &SoundAttenuation->Attenuation.CustomHighpassAirAbsorptionCurve.EditorCurveData;
			TArray<FRichCurveKey> tempKeys;
			int curvePreset = JsonAttenuationCurve->GetIntegerField(TEXT("preset"));

			switch (curvePreset)
			{
			default:
			case 0: //Linear preset
				{
					SoundAttenuation->Attenuation.DistanceAlgorithm = EAttenuationDistanceModel::Linear;
					SoundAttenuation->Attenuation.AbsorptionMethod = EAirAbsorptionMethod::Linear;
				}
				break;

			case 1: //Logarithmic preset
				{
					SoundAttenuation->Attenuation.DistanceAlgorithm = EAttenuationDistanceModel::Logarithmic;
					SoundAttenuation->Attenuation.AbsorptionMethod = EAirAbsorptionMethod::CustomCurve;
					FKeyHandle LogH1, LogH2;

					LogH1 = lowPassCurve->AddKey(0, 1);
					lowPassCurve->SetKeyTangentMode(LogH1, ERichCurveTangentMode::RCTM_Break);
					lowPassCurve->SetKeyInterpMode(LogH1, ERichCurveInterpMode::RCIM_Cubic);

					LogH2 = lowPassCurve->AddKey(1, 0);
					lowPassCurve->SetKeyTangentMode(LogH2, ERichCurveTangentMode::RCTM_Break);
					lowPassCurve->SetKeyInterpMode(LogH2, ERichCurveInterpMode::RCIM_Cubic);

					tempKeys = lowPassCurve->Keys;

					tempKeys[0].ArriveTangent = 0.0f;
					tempKeys[0].LeaveTangent = -2.302f;
					tempKeys[1].ArriveTangent = -0.105f;
					tempKeys[1].LeaveTangent = 0.0f;

					lowPassCurve->SetKeys(tempKeys);
				}
				break;

			case 2: // Inverse preset
				{
					SoundAttenuation->Attenuation.DistanceAlgorithm = EAttenuationDistanceModel::Inverse;
					SoundAttenuation->Attenuation.AbsorptionMethod = EAirAbsorptionMethod::CustomCurve;
					FKeyHandle InvH1, InvH2;

					InvH1 = lowPassCurve->AddKey(0, 1);
					lowPassCurve->SetKeyTangentMode(InvH1, ERichCurveTangentMode::RCTM_Break);
					lowPassCurve->SetKeyInterpMode(InvH1, ERichCurveInterpMode::RCIM_Cubic);

					InvH2 = lowPassCurve->AddKey(1, 0);
					lowPassCurve->SetKeyTangentMode(InvH2, ERichCurveTangentMode::RCTM_Break);
					lowPassCurve->SetKeyInterpMode(InvH2, ERichCurveInterpMode::RCIM_Cubic);

					tempKeys = lowPassCurve->Keys;

					tempKeys[0].ArriveTangent = 0.0f;
					tempKeys[0].LeaveTangent = -3.0f;
					tempKeys[1].ArriveTangent = 0.0f;
					tempKeys[1].LeaveTangent = 0.0f;

					lowPassCurve->SetKeys(tempKeys);
				}
				break;

			case 3: // No preset (Custom)
				{
					SoundAttenuation->Attenuation.DistanceAlgorithm = EAttenuationDistanceModel::Custom;


					TSharedPtr<FJsonObject> JsonCurveValue = JsonAttenuationCurve->GetObjectField(TEXT("curve"));
					TArray<TSharedPtr<FJsonValue>> JsonKeys = JsonCurveValue->GetArrayField(TEXT("keys"));

					int length = JsonCurveValue->GetIntegerField(TEXT("length"));
					int preWrapMode = JsonCurveValue->GetIntegerField(TEXT("preWrapMode"));
					int postWrapMode = JsonCurveValue->GetIntegerField(TEXT("postWrapMode"));

					for (int i = 0; i < length; i++)
					{
						TSharedPtr<FJsonObject> JsonKey = JsonKeys[i]->AsObject();
						float time = (float)JsonKey->GetNumberField(TEXT("time"));
						float value = (float)JsonKey->GetNumberField(TEXT("value"));
						auto handle = curve->AddKey(time, value);
						ERichCurveTangentMode RCTM = (ERichCurveTangentMode)JsonKey->
							GetIntegerField(TEXT("tangentMode"));
						// TO DO : Check if Unity's tangent mode equals UE's one
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

					SoundAttenuation->Attenuation.AbsorptionMethod = EAirAbsorptionMethod::CustomCurve;
					
					for (int i = 0; i < length; i++)
					{
						TSharedPtr<FJsonObject> JsonKey = JsonKeys[i]->AsObject();
						float time = (float)JsonKey->GetNumberField(TEXT("time"));
						float value = (float)JsonKey->GetNumberField(TEXT("value"));
						auto handle = lowPassCurve->AddKey(time, value);
						ERichCurveTangentMode RCTM = (ERichCurveTangentMode)JsonKey->
							GetIntegerField(TEXT("tangentMode"));
						// TO DO : Check if Unity's tangent mode equals UE's one
						ERichCurveInterpMode RCIM = ERichCurveInterpMode::RCIM_Cubic; // Not right, just placeholder
						//(ERichCurveInterpMode)JsonKey->GetIntegerField("weightedMode"); // Not right, just placeholder
						lowPassCurve->SetKeyTangentMode(handle, ERichCurveTangentMode::RCTM_Break);
						lowPassCurve->SetKeyInterpMode(handle, ERichCurveInterpMode::RCIM_Cubic);
					}

					tempKeys = lowPassCurve->Keys;

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

					lowPassCurve->SetKeys(tempKeys);
				}
				break;
			}

			SoundAttenuation->MarkPackageDirty();
			FAssetRegistryModule::AssetCreated(SoundAttenuation);

			Attenuation = FGisbAttenuation{
				JsonAttenuation->GetBoolField(TEXT("overrideParent")),
				SoundAttenuation,
			};
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to create package for attenuation asset: %s"), *AssetPath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid package name: %s"), *AssetPath);
	}


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
