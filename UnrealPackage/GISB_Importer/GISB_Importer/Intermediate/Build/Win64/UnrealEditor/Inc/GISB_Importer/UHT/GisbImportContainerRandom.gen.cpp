// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbImportContainerRandom.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbImportContainerRandom() {}

// Begin Cross Module References
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBase();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBase_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerRandom();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerRandom_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbImportContainerRandom
void UGisbImportContainerRandom::StaticRegisterNativesUGisbImportContainerRandom()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbImportContainerRandom);
UClass* Z_Construct_UClass_UGisbImportContainerRandom_NoRegister()
{
	return UGisbImportContainerRandom::StaticClass();
}
struct Z_Construct_UClass_UGisbImportContainerRandom_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbImportContainerRandom.h" },
		{ "ModuleRelativePath", "GisbImportContainerRandom.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SoundArray_Inner_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbImportContainerRandom.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SoundArray_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbImportContainerRandom.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AvoidLastPlayed_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbImportContainerRandom.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SoundArray_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_SoundArray;
	static const UECodeGen_Private::FIntPropertyParams NewProp_AvoidLastPlayed;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbImportContainerRandom>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UGisbImportContainerRandom_Statics::NewProp_SoundArray_Inner = { "SoundArray", nullptr, (EPropertyFlags)0x0002000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UGisbImportContainerBase_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SoundArray_Inner_MetaData), NewProp_SoundArray_Inner_MetaData) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UGisbImportContainerRandom_Statics::NewProp_SoundArray = { "SoundArray", nullptr, (EPropertyFlags)0x001000800000001d, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerRandom, SoundArray), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SoundArray_MetaData), NewProp_SoundArray_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UGisbImportContainerRandom_Statics::NewProp_AvoidLastPlayed = { "AvoidLastPlayed", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerRandom, AvoidLastPlayed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AvoidLastPlayed_MetaData), NewProp_AvoidLastPlayed_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbImportContainerRandom_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerRandom_Statics::NewProp_SoundArray_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerRandom_Statics::NewProp_SoundArray,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerRandom_Statics::NewProp_AvoidLastPlayed,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerRandom_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbImportContainerRandom_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGisbImportContainerBase,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerRandom_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbImportContainerRandom_Statics::ClassParams = {
	&UGisbImportContainerRandom::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbImportContainerRandom_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerRandom_Statics::PropPointers),
	0,
	0x008010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerRandom_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbImportContainerRandom_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbImportContainerRandom()
{
	if (!Z_Registration_Info_UClass_UGisbImportContainerRandom.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbImportContainerRandom.OuterSingleton, Z_Construct_UClass_UGisbImportContainerRandom_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbImportContainerRandom.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbImportContainerRandom>()
{
	return UGisbImportContainerRandom::StaticClass();
}
UGisbImportContainerRandom::UGisbImportContainerRandom(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbImportContainerRandom);
UGisbImportContainerRandom::~UGisbImportContainerRandom() {}
// End Class UGisbImportContainerRandom

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerRandom_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbImportContainerRandom, UGisbImportContainerRandom::StaticClass, TEXT("UGisbImportContainerRandom"), &Z_Registration_Info_UClass_UGisbImportContainerRandom, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbImportContainerRandom), 2662454522U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerRandom_h_1459265945(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerRandom_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerRandom_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
