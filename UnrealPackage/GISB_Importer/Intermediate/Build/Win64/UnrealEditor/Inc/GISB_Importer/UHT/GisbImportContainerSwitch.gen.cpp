// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbImportContainerSwitch.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbImportContainerSwitch() {}

// Begin Cross Module References
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBase();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBase_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerSwitch();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerSwitch_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbImportContainerSwitch
void UGisbImportContainerSwitch::StaticRegisterNativesUGisbImportContainerSwitch()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbImportContainerSwitch);
UClass* Z_Construct_UClass_UGisbImportContainerSwitch_NoRegister()
{
	return UGisbImportContainerSwitch::StaticClass();
}
struct Z_Construct_UClass_UGisbImportContainerSwitch_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbImportContainerSwitch.h" },
		{ "ModuleRelativePath", "GisbImportContainerSwitch.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ParameterID_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbImportContainerSwitch.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SoundDictionary_ValueProp_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbImportContainerSwitch.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SoundDictionary_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbImportContainerSwitch.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_ParameterID;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SoundDictionary_ValueProp;
	static const UECodeGen_Private::FStrPropertyParams NewProp_SoundDictionary_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_SoundDictionary;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbImportContainerSwitch>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UGisbImportContainerSwitch_Statics::NewProp_ParameterID = { "ParameterID", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerSwitch, ParameterID), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ParameterID_MetaData), NewProp_ParameterID_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UGisbImportContainerSwitch_Statics::NewProp_SoundDictionary_ValueProp = { "SoundDictionary", nullptr, (EPropertyFlags)0x0002000000080009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UGisbImportContainerBase_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SoundDictionary_ValueProp_MetaData), NewProp_SoundDictionary_ValueProp_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UGisbImportContainerSwitch_Statics::NewProp_SoundDictionary_Key_KeyProp = { "SoundDictionary_Key", nullptr, (EPropertyFlags)0x0002000000080009, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UGisbImportContainerSwitch_Statics::NewProp_SoundDictionary = { "SoundDictionary", nullptr, (EPropertyFlags)0x001000800000001d, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerSwitch, SoundDictionary), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SoundDictionary_MetaData), NewProp_SoundDictionary_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbImportContainerSwitch_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerSwitch_Statics::NewProp_ParameterID,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerSwitch_Statics::NewProp_SoundDictionary_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerSwitch_Statics::NewProp_SoundDictionary_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerSwitch_Statics::NewProp_SoundDictionary,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerSwitch_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbImportContainerSwitch_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGisbImportContainerBase,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerSwitch_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbImportContainerSwitch_Statics::ClassParams = {
	&UGisbImportContainerSwitch::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbImportContainerSwitch_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerSwitch_Statics::PropPointers),
	0,
	0x008010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerSwitch_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbImportContainerSwitch_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbImportContainerSwitch()
{
	if (!Z_Registration_Info_UClass_UGisbImportContainerSwitch.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbImportContainerSwitch.OuterSingleton, Z_Construct_UClass_UGisbImportContainerSwitch_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbImportContainerSwitch.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbImportContainerSwitch>()
{
	return UGisbImportContainerSwitch::StaticClass();
}
UGisbImportContainerSwitch::UGisbImportContainerSwitch(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbImportContainerSwitch);
UGisbImportContainerSwitch::~UGisbImportContainerSwitch() {}
// End Class UGisbImportContainerSwitch

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerSwitch_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbImportContainerSwitch, UGisbImportContainerSwitch::StaticClass, TEXT("UGisbImportContainerSwitch"), &Z_Registration_Info_UClass_UGisbImportContainerSwitch, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbImportContainerSwitch), 421228901U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerSwitch_h_3426660458(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerSwitch_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerSwitch_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
