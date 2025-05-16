// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbContainerBase.h"
#include "GISB_Importer/GisbStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbContainerBase() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerBase();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerBase_NoRegister();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbAttenuation();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbLowPass();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbPitch();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbVolume();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbContainerBase
void UGisbContainerBase::StaticRegisterNativesUGisbContainerBase()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbContainerBase);
UClass* Z_Construct_UClass_UGisbContainerBase_NoRegister()
{
	return UGisbContainerBase::StaticClass();
}
struct Z_Construct_UClass_UGisbContainerBase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbContainerBase.h" },
		{ "ModuleRelativePath", "GisbContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Attenuation_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_VolumeDB_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Pitch_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Lowpass_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PlaybackProbabilityPercent_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbContainerBase.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStructPropertyParams NewProp_Attenuation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_VolumeDB;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Pitch;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Lowpass;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PlaybackProbabilityPercent;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbContainerBase>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UGisbContainerBase_Statics::NewProp_Attenuation = { "Attenuation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerBase, Attenuation), Z_Construct_UScriptStruct_FGisbAttenuation, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Attenuation_MetaData), NewProp_Attenuation_MetaData) }; // 615850082
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UGisbContainerBase_Statics::NewProp_VolumeDB = { "VolumeDB", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerBase, VolumeDB), Z_Construct_UScriptStruct_FGisbVolume, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_VolumeDB_MetaData), NewProp_VolumeDB_MetaData) }; // 2798983610
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UGisbContainerBase_Statics::NewProp_Pitch = { "Pitch", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerBase, Pitch), Z_Construct_UScriptStruct_FGisbPitch, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Pitch_MetaData), NewProp_Pitch_MetaData) }; // 1570584861
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UGisbContainerBase_Statics::NewProp_Lowpass = { "Lowpass", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerBase, Lowpass), Z_Construct_UScriptStruct_FGisbLowPass, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Lowpass_MetaData), NewProp_Lowpass_MetaData) }; // 279662145
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UGisbContainerBase_Statics::NewProp_PlaybackProbabilityPercent = { "PlaybackProbabilityPercent", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerBase, PlaybackProbabilityPercent), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PlaybackProbabilityPercent_MetaData), NewProp_PlaybackProbabilityPercent_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbContainerBase_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerBase_Statics::NewProp_Attenuation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerBase_Statics::NewProp_VolumeDB,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerBase_Statics::NewProp_Pitch,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerBase_Statics::NewProp_Lowpass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerBase_Statics::NewProp_PlaybackProbabilityPercent,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerBase_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbContainerBase_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerBase_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbContainerBase_Statics::ClassParams = {
	&UGisbContainerBase::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbContainerBase_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerBase_Statics::PropPointers),
	0,
	0x002010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerBase_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbContainerBase_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbContainerBase()
{
	if (!Z_Registration_Info_UClass_UGisbContainerBase.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbContainerBase.OuterSingleton, Z_Construct_UClass_UGisbContainerBase_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbContainerBase.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbContainerBase>()
{
	return UGisbContainerBase::StaticClass();
}
UGisbContainerBase::UGisbContainerBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbContainerBase);
UGisbContainerBase::~UGisbContainerBase() {}
// End Class UGisbContainerBase

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerBase_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbContainerBase, UGisbContainerBase::StaticClass, TEXT("UGisbContainerBase"), &Z_Registration_Info_UClass_UGisbContainerBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbContainerBase), 1589389159U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerBase_h_2518191938(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerBase_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
