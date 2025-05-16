// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbImportContainerBlend.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbImportContainerBlend() {}

// Begin Cross Module References
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBase();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBase_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBlend();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBlend_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbImportContainerBlend
void UGisbImportContainerBlend::StaticRegisterNativesUGisbImportContainerBlend()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbImportContainerBlend);
UClass* Z_Construct_UClass_UGisbImportContainerBlend_NoRegister()
{
	return UGisbImportContainerBlend::StaticClass();
}
struct Z_Construct_UClass_UGisbImportContainerBlend_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n *\n */" },
		{ "IncludePath", "GisbImportContainerBlend.h" },
		{ "ModuleRelativePath", "GisbImportContainerBlend.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SoundArray_Inner_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbImportContainerBlend.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SoundArray_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbImportContainerBlend.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SoundArray_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_SoundArray;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbImportContainerBlend>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UGisbImportContainerBlend_Statics::NewProp_SoundArray_Inner = { "SoundArray", nullptr, (EPropertyFlags)0x0002000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UGisbImportContainerBase_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SoundArray_Inner_MetaData), NewProp_SoundArray_Inner_MetaData) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UGisbImportContainerBlend_Statics::NewProp_SoundArray = { "SoundArray", nullptr, (EPropertyFlags)0x001000800000001d, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerBlend, SoundArray), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SoundArray_MetaData), NewProp_SoundArray_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbImportContainerBlend_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerBlend_Statics::NewProp_SoundArray_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerBlend_Statics::NewProp_SoundArray,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerBlend_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbImportContainerBlend_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGisbImportContainerBase,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerBlend_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbImportContainerBlend_Statics::ClassParams = {
	&UGisbImportContainerBlend::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbImportContainerBlend_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerBlend_Statics::PropPointers),
	0,
	0x008010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerBlend_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbImportContainerBlend_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbImportContainerBlend()
{
	if (!Z_Registration_Info_UClass_UGisbImportContainerBlend.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbImportContainerBlend.OuterSingleton, Z_Construct_UClass_UGisbImportContainerBlend_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbImportContainerBlend.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbImportContainerBlend>()
{
	return UGisbImportContainerBlend::StaticClass();
}
UGisbImportContainerBlend::UGisbImportContainerBlend(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbImportContainerBlend);
UGisbImportContainerBlend::~UGisbImportContainerBlend() {}
// End Class UGisbImportContainerBlend

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerBlend_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbImportContainerBlend, UGisbImportContainerBlend::StaticClass, TEXT("UGisbImportContainerBlend"), &Z_Registration_Info_UClass_UGisbImportContainerBlend, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbImportContainerBlend), 1803006909U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerBlend_h_4194459167(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerBlend_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerBlend_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
