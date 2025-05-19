// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbSoundBankDataAsset.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbSoundBankDataAsset() {}

// Begin Cross Module References
COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FPrimaryAssetType();
ENGINE_API UClass* Z_Construct_UClass_UPrimaryDataAsset();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerBase_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbSoundBankDataAsset();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbSoundBankDataAsset_NoRegister();
METASOUNDENGINE_API UClass* Z_Construct_UClass_UMetaSoundSource_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbSoundBankDataAsset
void UGisbSoundBankDataAsset::StaticRegisterNativesUGisbSoundBankDataAsset()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbSoundBankDataAsset);
UClass* Z_Construct_UClass_UGisbSoundBankDataAsset_NoRegister()
{
	return UGisbSoundBankDataAsset::StaticClass();
}
struct Z_Construct_UClass_UGisbSoundBankDataAsset_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbSoundBankDataAsset.h" },
		{ "ModuleRelativePath", "GisbSoundBankDataAsset.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_EventName_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbSoundBankDataAsset.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_GeneratedMS_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbSoundBankDataAsset.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_RootContainer_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbSoundBankDataAsset.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AssetType_MetaData[] = {
		{ "Category", "Asset Info" },
		{ "ModuleRelativePath", "GisbSoundBankDataAsset.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_EventName;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_GeneratedMS;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_RootContainer;
	static const UECodeGen_Private::FStructPropertyParams NewProp_AssetType;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbSoundBankDataAsset>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::NewProp_EventName = { "EventName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbSoundBankDataAsset, EventName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_EventName_MetaData), NewProp_EventName_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::NewProp_GeneratedMS = { "GeneratedMS", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbSoundBankDataAsset, GeneratedMS), Z_Construct_UClass_UMetaSoundSource_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_GeneratedMS_MetaData), NewProp_GeneratedMS_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::NewProp_RootContainer = { "RootContainer", nullptr, (EPropertyFlags)0x001200000008000d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbSoundBankDataAsset, RootContainer), Z_Construct_UClass_UGisbContainerBase_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_RootContainer_MetaData), NewProp_RootContainer_MetaData) };
const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::NewProp_AssetType = { "AssetType", nullptr, (EPropertyFlags)0x0010000000020015, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbSoundBankDataAsset, AssetType), Z_Construct_UScriptStruct_FPrimaryAssetType, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AssetType_MetaData), NewProp_AssetType_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::NewProp_EventName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::NewProp_GeneratedMS,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::NewProp_RootContainer,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::NewProp_AssetType,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UPrimaryDataAsset,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::ClassParams = {
	&UGisbSoundBankDataAsset::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::PropPointers),
	0,
	0x008000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbSoundBankDataAsset()
{
	if (!Z_Registration_Info_UClass_UGisbSoundBankDataAsset.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbSoundBankDataAsset.OuterSingleton, Z_Construct_UClass_UGisbSoundBankDataAsset_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbSoundBankDataAsset.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbSoundBankDataAsset>()
{
	return UGisbSoundBankDataAsset::StaticClass();
}
UGisbSoundBankDataAsset::UGisbSoundBankDataAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbSoundBankDataAsset);
UGisbSoundBankDataAsset::~UGisbSoundBankDataAsset() {}
// End Class UGisbSoundBankDataAsset

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankDataAsset_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbSoundBankDataAsset, UGisbSoundBankDataAsset::StaticClass, TEXT("UGisbSoundBankDataAsset"), &Z_Registration_Info_UClass_UGisbSoundBankDataAsset, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbSoundBankDataAsset), 2162547215U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankDataAsset_h_3121501241(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankDataAsset_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankDataAsset_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
