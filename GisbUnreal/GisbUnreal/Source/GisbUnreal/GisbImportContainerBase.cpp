// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbImportContainerBase.h"
#include "Dom/JsonObject.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundFrontendDataTypeRegistry.h"
/*#include "MetasoundFrontendVersion.h"
#include "MetasoundFrontendRegistryContainer.h"
#include "MetasoundFrontendDocumentBuilder.h"*/
#include "MetasoundFrontendArchetypeRegistry.h"
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

	FRuntimeFloatCurve* AttenuationCurve = new FRuntimeFloatCurve();
	FRichCurve* curve = &AttenuationCurve->EditorCurveData;
	TArray<FRichCurveKey> tempKeys;
	int curvePreset = JsonAttenuationValue->GetIntegerField("preset");

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
			TSharedPtr<FJsonObject> JsonCurveValue = JsonAttenuationValue->GetObjectField("curve");
			TArray<TSharedPtr<FJsonValue>> JsonKeys = JsonCurveValue->GetArrayField("keys");

			int length = JsonCurveValue->GetIntegerField("length");
			int preWrapMode = JsonCurveValue->GetIntegerField("preWrapMode");
			int postWrapMode = JsonCurveValue->GetIntegerField("postWrapMode");
			
			for(int i = 0; i<length; i++)
			{
				TSharedPtr<FJsonObject> JsonKey = JsonKeys[i]->AsObject();
				float time = (float)JsonKey->GetNumberField("time");
				float value = (float)JsonKey->GetNumberField("value");
				auto handle = curve->AddKey(time, value);
				ERichCurveTangentMode RCTM = (ERichCurveTangentMode)JsonKey->GetIntegerField("tangentMode"); // TO DO : Check if Unity's tangent mode equals UE's one
				ERichCurveInterpMode RCIM = ERichCurveInterpMode::RCIM_Cubic; // Not right, just placeholder
				//(ERichCurveInterpMode)JsonKey->GetIntegerField("weightedMode"); // Not right, just placeholder
				curve->SetKeyTangentMode(handle, ERichCurveTangentMode::RCTM_Break);
				curve->SetKeyInterpMode(handle, ERichCurveInterpMode::RCIM_Cubic);
			}

			tempKeys = curve->Keys;

			for (int i = 0; i < length; i++)
			{

				TSharedPtr<FJsonObject> JsonKey = JsonKeys[i]->AsObject();

				float inTngnt = (float)JsonKey->GetNumberField("inTangent");
				float outTngnt = (float)JsonKey->GetNumberField("outTangent");
				float inWeight = (float)JsonKey->GetNumberField("inWeight");
				float outWeight = (float)JsonKey->GetNumberField("outWeight");
				
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
		JsonAttenuation->GetBoolField("overrideParent"),
		JsonAttenuationValue->GetBoolField("active"),
		(float)JsonAttenuationValue->GetNumberField("minDistance"),
		(float)JsonAttenuationValue->GetNumberField("maxDistance"),
		*AttenuationCurve,
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

void UGisbImportContainerBase::AssignBaseVariables(UGisbContainerBase* Container) const
{
	Container->Attenuation = Attenuation;
	Container->VolumeDB = VolumeDB;
	Container->Pitch = Pitch;
	Container->Lowpass = Lowpass;
	Container->PlaybackProbabilityPercent = PlaybackProbabilityPercent;
}

FMetasoundFrontendDocument UGisbImportContainerBase::ToMSDocument(FString Name)  
{  
  FMetasoundFrontendDocument Document;

  FMetasoundFrontendClassMetadata Metadata = FMetasoundFrontendClassMetadata();
  Metadata.SetDisplayName(FText::FromString(Name));
  Metadata.SetAuthor("Generated via C++");
  Metadata.SetType(EMetasoundFrontendClassType::Graph);

  Document.RootGraph.Metadata = Metadata;
  
  /*
  // Node registry
  FMetasoundFrontendClassInputLiteral FrequencyInput;
  FrequencyInput.Name = TEXT("Frequency");
  FrequencyInput.TypeName = "Float";
  FrequencyInput.Value = MakeLiteral(440.0f);
  */
  const FMetasoundFrontendClassName ClassName{ TEXT("UE"), TEXT("Sine"), TEXT("Audio") };
  EMetasoundFrontendClassType ClassType = EMetasoundFrontendClassType::External;

  FMetasoundFrontendClass SineClass;
  Metasound::Frontend::FNodeRegistryKey key;
  key = Metasound::Frontend::NodeRegistryKey::CreateKey(ClassType, ClassName.ToString(), 1, 0);
  FMetasoundFrontendRegistryContainer::Get()->FindFrontendClassFromRegistered(key, SineClass);

  // Add sine oscillator node
  FMetasoundFrontendNode PitchNode;
  PitchNode.ClassID = SineClass.ID /*FGuid("C3374D90426FC65520BE6CAC665B0A65")*/;
  //PitchNode.UpdateID(FGuid("C3374D90426FC65520BE6CAC665B0A65"));
  PitchNode.Name = TEXT("GISB_Test");
  FGuid id = PitchNode.GetID();
  Document.RootGraph.Graph.Nodes.Add(PitchNode);

  /*
  // Add output node
  FMetasoundFrontendNode OutputNode;
  OutputNode.ClassName = {
	  "UE",
	  "AudioOutput",
	  "Audio"
  };
  OutputNode.Name = TEXT("OutputNode");
  Document.RootGraph.Nodes.Add(OutputNode);

  // Connect sine -> output
  FMetasoundFrontendEdge Edge;
  Edge.FromNode = PitchNode.Name;
  Edge.FromVertex = TEXT("Audio");
  Edge.ToNode = OutputNode.Name;
  Edge.ToVertex = TEXT("Audio");
  Document.RootGraph.Edges.Add(Edge);

  */
  // TODO: Implement the conversion to Metasound document  
  // FMetasoundFrontendRegistryContainer::Get();  
  return Document;
}
