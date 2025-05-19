// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbContainerRandom.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbContainerRandom() {}

// Begin Cross Module References
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerBase();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerBase_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerRandom();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerRandom_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbContainerRandom
void UGisbContainerRandom::StaticRegisterNativesUGisbContainerRandom()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbContainerRandom);
UClass* Z_Construct_UClass_UGisbContainerRandom_NoRegister()
{
	return UGisbContainerRandom::StaticClass();
}
struct Z_Construct_UClass_UGisbContainerRandom_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbContainerRandom.h" },
		{ "ModuleRelativePath", "GisbContainerRandom.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ChildContainers_Inner_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbContainerRandom.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ChildContainers_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbContainerRandom.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AvoidLastPlayed_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbContainerRandom.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ChildContainers_Inner;
	static const UECodeGen_Private::FArrayPropertyParams NewProp_ChildContainers;
	static const UECodeGen_Private::FIntPropertyParams NewProp_AvoidLastPlayed;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbContainerRandom>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UGisbContainerRandom_Statics::NewProp_ChildContainers_Inner = { "ChildContainers", nullptr, (EPropertyFlags)0x0002000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, Z_Construct_UClass_UGisbContainerBase_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ChildContainers_Inner_MetaData), NewProp_ChildContainers_Inner_MetaData) };
const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UGisbContainerRandom_Statics::NewProp_ChildContainers = { "ChildContainers", nullptr, (EPropertyFlags)0x001000800000001d, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerRandom, ChildContainers), EArrayPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ChildContainers_MetaData), NewProp_ChildContainers_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UGisbContainerRandom_Statics::NewProp_AvoidLastPlayed = { "AvoidLastPlayed", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerRandom, AvoidLastPlayed), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AvoidLastPlayed_MetaData), NewProp_AvoidLastPlayed_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbContainerRandom_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerRandom_Statics::NewProp_ChildContainers_Inner,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerRandom_Statics::NewProp_ChildContainers,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerRandom_Statics::NewProp_AvoidLastPlayed,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerRandom_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbContainerRandom_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGisbContainerBase,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerRandom_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbContainerRandom_Statics::ClassParams = {
	&UGisbContainerRandom::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbContainerRandom_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerRandom_Statics::PropPointers),
	0,
	0x00A010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerRandom_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbContainerRandom_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbContainerRandom()
{
	if (!Z_Registration_Info_UClass_UGisbContainerRandom.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbContainerRandom.OuterSingleton, Z_Construct_UClass_UGisbContainerRandom_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbContainerRandom.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbContainerRandom>()
{
	return UGisbContainerRandom::StaticClass();
}
UGisbContainerRandom::UGisbContainerRandom(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbContainerRandom);
UGisbContainerRandom::~UGisbContainerRandom() {}
// End Class UGisbContainerRandom

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerRandom_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbContainerRandom, UGisbContainerRandom::StaticClass, TEXT("UGisbContainerRandom"), &Z_Registration_Info_UClass_UGisbContainerRandom, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbContainerRandom), 3454040313U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerRandom_h_212721820(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerRandom_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerRandom_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
