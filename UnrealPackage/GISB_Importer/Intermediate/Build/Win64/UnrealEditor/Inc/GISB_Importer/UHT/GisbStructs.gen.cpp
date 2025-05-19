// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbStructs.h"
#include "Runtime/Engine/Classes/Curves/CurveFloat.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbStructs() {}

// Begin Cross Module References
ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FRuntimeFloatCurve();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbAttenuation();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbLowPass();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbPitch();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbVolume();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin ScriptStruct FGisbAttenuation
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_GisbAttenuation;
class UScriptStruct* FGisbAttenuation::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_GisbAttenuation.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_GisbAttenuation.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FGisbAttenuation, (UObject*)Z_Construct_UPackage__Script_GISB_Importer(), TEXT("GisbAttenuation"));
	}
	return Z_Registration_Info_UScriptStruct_GisbAttenuation.OuterSingleton;
}
template<> GISB_IMPORTER_API UScriptStruct* StaticStruct<FGisbAttenuation>()
{
	return FGisbAttenuation::StaticStruct();
}
struct Z_Construct_UScriptStruct_FGisbAttenuation_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_overrideParent_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_active_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_minDistance_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_maxDistance_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_attenuationCurve_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_volumeAtMax_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_spreadAtMin_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_spreadAtMax_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_lowPassAtMin_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_lowPassAtMax_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
#endif // WITH_METADATA
	static void NewProp_overrideParent_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_overrideParent;
	static void NewProp_active_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_active;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_minDistance;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_maxDistance;
	static const UECodeGen_Private::FStructPropertyParams NewProp_attenuationCurve;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_volumeAtMax;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_spreadAtMin;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_spreadAtMax;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_lowPassAtMin;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_lowPassAtMax;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FGisbAttenuation>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
void Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_overrideParent_SetBit(void* Obj)
{
	((FGisbAttenuation*)Obj)->overrideParent = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_overrideParent = { "overrideParent", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FGisbAttenuation), &Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_overrideParent_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_overrideParent_MetaData), NewProp_overrideParent_MetaData) };
void Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_active_SetBit(void* Obj)
{
	((FGisbAttenuation*)Obj)->active = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_active = { "active", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FGisbAttenuation), &Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_active_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_active_MetaData), NewProp_active_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_minDistance = { "minDistance", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbAttenuation, minDistance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_minDistance_MetaData), NewProp_minDistance_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_maxDistance = { "maxDistance", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbAttenuation, maxDistance), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_maxDistance_MetaData), NewProp_maxDistance_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_attenuationCurve = { "attenuationCurve", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbAttenuation, attenuationCurve), Z_Construct_UScriptStruct_FRuntimeFloatCurve, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_attenuationCurve_MetaData), NewProp_attenuationCurve_MetaData) }; // 1495033350
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_volumeAtMax = { "volumeAtMax", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbAttenuation, volumeAtMax), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_volumeAtMax_MetaData), NewProp_volumeAtMax_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_spreadAtMin = { "spreadAtMin", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbAttenuation, spreadAtMin), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_spreadAtMin_MetaData), NewProp_spreadAtMin_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_spreadAtMax = { "spreadAtMax", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbAttenuation, spreadAtMax), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_spreadAtMax_MetaData), NewProp_spreadAtMax_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_lowPassAtMin = { "lowPassAtMin", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbAttenuation, lowPassAtMin), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_lowPassAtMin_MetaData), NewProp_lowPassAtMin_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_lowPassAtMax = { "lowPassAtMax", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbAttenuation, lowPassAtMax), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_lowPassAtMax_MetaData), NewProp_lowPassAtMax_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FGisbAttenuation_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_overrideParent,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_active,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_minDistance,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_maxDistance,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_attenuationCurve,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_volumeAtMax,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_spreadAtMin,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_spreadAtMax,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_lowPassAtMin,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewProp_lowPassAtMax,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbAttenuation_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FGisbAttenuation_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
	nullptr,
	&NewStructOps,
	"GisbAttenuation",
	Z_Construct_UScriptStruct_FGisbAttenuation_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbAttenuation_Statics::PropPointers),
	sizeof(FGisbAttenuation),
	alignof(FGisbAttenuation),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbAttenuation_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FGisbAttenuation_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FGisbAttenuation()
{
	if (!Z_Registration_Info_UScriptStruct_GisbAttenuation.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_GisbAttenuation.InnerSingleton, Z_Construct_UScriptStruct_FGisbAttenuation_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_GisbAttenuation.InnerSingleton;
}
// End ScriptStruct FGisbAttenuation

// Begin ScriptStruct FGisbVolume
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_GisbVolume;
class UScriptStruct* FGisbVolume::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_GisbVolume.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_GisbVolume.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FGisbVolume, (UObject*)Z_Construct_UPackage__Script_GISB_Importer(), TEXT("GisbVolume"));
	}
	return Z_Registration_Info_UScriptStruct_GisbVolume.OuterSingleton;
}
template<> GISB_IMPORTER_API UScriptStruct* StaticStruct<FGisbVolume>()
{
	return FGisbVolume::StaticStruct();
}
struct Z_Construct_UScriptStruct_FGisbVolume_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_value_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_randomize_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_minRnd_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_maxRnd_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_value;
	static void NewProp_randomize_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_randomize;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_minRnd;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_maxRnd;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FGisbVolume>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbVolume_Statics::NewProp_value = { "value", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbVolume, value), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_value_MetaData), NewProp_value_MetaData) };
void Z_Construct_UScriptStruct_FGisbVolume_Statics::NewProp_randomize_SetBit(void* Obj)
{
	((FGisbVolume*)Obj)->randomize = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FGisbVolume_Statics::NewProp_randomize = { "randomize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FGisbVolume), &Z_Construct_UScriptStruct_FGisbVolume_Statics::NewProp_randomize_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_randomize_MetaData), NewProp_randomize_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbVolume_Statics::NewProp_minRnd = { "minRnd", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbVolume, minRnd), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_minRnd_MetaData), NewProp_minRnd_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbVolume_Statics::NewProp_maxRnd = { "maxRnd", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbVolume, maxRnd), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_maxRnd_MetaData), NewProp_maxRnd_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FGisbVolume_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbVolume_Statics::NewProp_value,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbVolume_Statics::NewProp_randomize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbVolume_Statics::NewProp_minRnd,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbVolume_Statics::NewProp_maxRnd,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbVolume_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FGisbVolume_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
	nullptr,
	&NewStructOps,
	"GisbVolume",
	Z_Construct_UScriptStruct_FGisbVolume_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbVolume_Statics::PropPointers),
	sizeof(FGisbVolume),
	alignof(FGisbVolume),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbVolume_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FGisbVolume_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FGisbVolume()
{
	if (!Z_Registration_Info_UScriptStruct_GisbVolume.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_GisbVolume.InnerSingleton, Z_Construct_UScriptStruct_FGisbVolume_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_GisbVolume.InnerSingleton;
}
// End ScriptStruct FGisbVolume

// Begin ScriptStruct FGisbPitch
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_GisbPitch;
class UScriptStruct* FGisbPitch::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_GisbPitch.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_GisbPitch.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FGisbPitch, (UObject*)Z_Construct_UPackage__Script_GISB_Importer(), TEXT("GisbPitch"));
	}
	return Z_Registration_Info_UScriptStruct_GisbPitch.OuterSingleton;
}
template<> GISB_IMPORTER_API UScriptStruct* StaticStruct<FGisbPitch>()
{
	return FGisbPitch::StaticStruct();
}
struct Z_Construct_UScriptStruct_FGisbPitch_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_value_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_randomize_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_minRnd_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_maxRnd_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_value;
	static void NewProp_randomize_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_randomize;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_minRnd;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_maxRnd;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FGisbPitch>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbPitch_Statics::NewProp_value = { "value", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbPitch, value), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_value_MetaData), NewProp_value_MetaData) };
void Z_Construct_UScriptStruct_FGisbPitch_Statics::NewProp_randomize_SetBit(void* Obj)
{
	((FGisbPitch*)Obj)->randomize = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FGisbPitch_Statics::NewProp_randomize = { "randomize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FGisbPitch), &Z_Construct_UScriptStruct_FGisbPitch_Statics::NewProp_randomize_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_randomize_MetaData), NewProp_randomize_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbPitch_Statics::NewProp_minRnd = { "minRnd", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbPitch, minRnd), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_minRnd_MetaData), NewProp_minRnd_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbPitch_Statics::NewProp_maxRnd = { "maxRnd", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbPitch, maxRnd), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_maxRnd_MetaData), NewProp_maxRnd_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FGisbPitch_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbPitch_Statics::NewProp_value,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbPitch_Statics::NewProp_randomize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbPitch_Statics::NewProp_minRnd,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbPitch_Statics::NewProp_maxRnd,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbPitch_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FGisbPitch_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
	nullptr,
	&NewStructOps,
	"GisbPitch",
	Z_Construct_UScriptStruct_FGisbPitch_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbPitch_Statics::PropPointers),
	sizeof(FGisbPitch),
	alignof(FGisbPitch),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbPitch_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FGisbPitch_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FGisbPitch()
{
	if (!Z_Registration_Info_UScriptStruct_GisbPitch.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_GisbPitch.InnerSingleton, Z_Construct_UScriptStruct_FGisbPitch_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_GisbPitch.InnerSingleton;
}
// End ScriptStruct FGisbPitch

// Begin ScriptStruct FGisbLowPass
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_GisbLowPass;
class UScriptStruct* FGisbLowPass::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_GisbLowPass.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_GisbLowPass.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FGisbLowPass, (UObject*)Z_Construct_UPackage__Script_GISB_Importer(), TEXT("GisbLowPass"));
	}
	return Z_Registration_Info_UScriptStruct_GisbLowPass.OuterSingleton;
}
template<> GISB_IMPORTER_API UScriptStruct* StaticStruct<FGisbLowPass>()
{
	return FGisbLowPass::StaticStruct();
}
struct Z_Construct_UScriptStruct_FGisbLowPass_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_value_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_randomize_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_minRnd_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_maxRnd_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbStructs.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFloatPropertyParams NewProp_value;
	static void NewProp_randomize_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_randomize;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_minRnd;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_maxRnd;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FGisbLowPass>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewProp_value = { "value", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbLowPass, value), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_value_MetaData), NewProp_value_MetaData) };
void Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewProp_randomize_SetBit(void* Obj)
{
	((FGisbLowPass*)Obj)->randomize = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewProp_randomize = { "randomize", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(FGisbLowPass), &Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewProp_randomize_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_randomize_MetaData), NewProp_randomize_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewProp_minRnd = { "minRnd", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbLowPass, minRnd), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_minRnd_MetaData), NewProp_minRnd_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewProp_maxRnd = { "maxRnd", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FGisbLowPass, maxRnd), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_maxRnd_MetaData), NewProp_maxRnd_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FGisbLowPass_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewProp_value,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewProp_randomize,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewProp_minRnd,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewProp_maxRnd,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbLowPass_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FGisbLowPass_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
	nullptr,
	&NewStructOps,
	"GisbLowPass",
	Z_Construct_UScriptStruct_FGisbLowPass_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbLowPass_Statics::PropPointers),
	sizeof(FGisbLowPass),
	alignof(FGisbLowPass),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FGisbLowPass_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FGisbLowPass_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FGisbLowPass()
{
	if (!Z_Registration_Info_UScriptStruct_GisbLowPass.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_GisbLowPass.InnerSingleton, Z_Construct_UScriptStruct_FGisbLowPass_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_GisbLowPass.InnerSingleton;
}
// End ScriptStruct FGisbLowPass

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbStructs_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FGisbAttenuation::StaticStruct, Z_Construct_UScriptStruct_FGisbAttenuation_Statics::NewStructOps, TEXT("GisbAttenuation"), &Z_Registration_Info_UScriptStruct_GisbAttenuation, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FGisbAttenuation), 615850082U) },
		{ FGisbVolume::StaticStruct, Z_Construct_UScriptStruct_FGisbVolume_Statics::NewStructOps, TEXT("GisbVolume"), &Z_Registration_Info_UScriptStruct_GisbVolume, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FGisbVolume), 2798983610U) },
		{ FGisbPitch::StaticStruct, Z_Construct_UScriptStruct_FGisbPitch_Statics::NewStructOps, TEXT("GisbPitch"), &Z_Registration_Info_UScriptStruct_GisbPitch, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FGisbPitch), 1570584861U) },
		{ FGisbLowPass::StaticStruct, Z_Construct_UScriptStruct_FGisbLowPass_Statics::NewStructOps, TEXT("GisbLowPass"), &Z_Registration_Info_UScriptStruct_GisbLowPass, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FGisbLowPass), 279662145U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbStructs_h_2309756001(TEXT("/Script/GISB_Importer"),
	nullptr, 0,
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbStructs_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbStructs_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
