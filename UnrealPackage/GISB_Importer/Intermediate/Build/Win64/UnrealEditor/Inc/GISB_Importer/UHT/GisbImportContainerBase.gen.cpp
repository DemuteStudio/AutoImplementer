// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbImportContainerBase.h"
#include "GISB_Importer/GisbStructs.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbImportContainerBase() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBase();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBase_NoRegister();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbAttenuation();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbLowPass();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbPitch();
GISB_IMPORTER_API UScriptStruct* Z_Construct_UScriptStruct_FGisbVolume();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbImportContainerBase
void UGisbImportContainerBase::StaticRegisterNativesUGisbImportContainerBase()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbImportContainerBase);
UClass* Z_Construct_UClass_UGisbImportContainerBase_NoRegister()
{
	return UGisbImportContainerBase::StaticClass();
}
struct Z_Construct_UClass_UGisbImportContainerBase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbImportContainerBase.h" },
		{ "ModuleRelativePath", "GisbImportContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Type_MetaData[] = {
		{ "ModuleRelativePath", "GisbImportContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Attenuation_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbImportContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_VolumeDB_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbImportContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Pitch_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbImportContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Lowpass_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbImportContainerBase.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_PlaybackProbabilityPercent_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbImportContainerBase.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_Type;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Attenuation;
	static const UECodeGen_Private::FStructPropertyParams NewProp_VolumeDB;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Pitch;
	static const UECodeGen_Private::FStructPropertyParams NewProp_Lowpass;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_PlaybackProbabilityPercent;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbImportContainerBase>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_Type = { "Type", nullptr, (EPropertyFlags)0x0010000000002000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerBase, Type), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Type_MetaData), NewProp_Type_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_Attenuation = { "Attenuation", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerBase, Attenuation), Z_Construct_UScriptStruct_FGisbAttenuation, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Attenuation_MetaData), NewProp_Attenuation_MetaData) }; // 615850082
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_VolumeDB = { "VolumeDB", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerBase, VolumeDB), Z_Construct_UScriptStruct_FGisbVolume, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_VolumeDB_MetaData), NewProp_VolumeDB_MetaData) }; // 2798983610
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_Pitch = { "Pitch", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerBase, Pitch), Z_Construct_UScriptStruct_FGisbPitch, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Pitch_MetaData), NewProp_Pitch_MetaData) }; // 1570584861
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_Lowpass = { "Lowpass", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerBase, Lowpass), Z_Construct_UScriptStruct_FGisbLowPass, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Lowpass_MetaData), NewProp_Lowpass_MetaData) }; // 279662145
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_PlaybackProbabilityPercent = { "PlaybackProbabilityPercent", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerBase, PlaybackProbabilityPercent), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_PlaybackProbabilityPercent_MetaData), NewProp_PlaybackProbabilityPercent_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbImportContainerBase_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_Type,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_Attenuation,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_VolumeDB,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_Pitch,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_Lowpass,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerBase_Statics::NewProp_PlaybackProbabilityPercent,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerBase_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbImportContainerBase_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerBase_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbImportContainerBase_Statics::ClassParams = {
	&UGisbImportContainerBase::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbImportContainerBase_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerBase_Statics::PropPointers),
	0,
	0x000010A1u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerBase_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbImportContainerBase_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbImportContainerBase()
{
	if (!Z_Registration_Info_UClass_UGisbImportContainerBase.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbImportContainerBase.OuterSingleton, Z_Construct_UClass_UGisbImportContainerBase_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbImportContainerBase.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbImportContainerBase>()
{
	return UGisbImportContainerBase::StaticClass();
}
UGisbImportContainerBase::UGisbImportContainerBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbImportContainerBase);
UGisbImportContainerBase::~UGisbImportContainerBase() {}
// End Class UGisbImportContainerBase

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerBase_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbImportContainerBase, UGisbImportContainerBase::StaticClass, TEXT("UGisbImportContainerBase"), &Z_Registration_Info_UClass_UGisbImportContainerBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbImportContainerBase), 3622804990U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerBase_h_50299493(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerBase_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
