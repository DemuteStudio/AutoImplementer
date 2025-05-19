// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbContainerSwitch.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbContainerSwitch() {}

// Begin Cross Module References
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerBase();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerBase_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerSwitch();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerSwitch_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbContainerSwitch
void UGisbContainerSwitch::StaticRegisterNativesUGisbContainerSwitch()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbContainerSwitch);
UClass* Z_Construct_UClass_UGisbContainerSwitch_NoRegister()
{
	return UGisbContainerSwitch::StaticClass();
}
struct Z_Construct_UClass_UGisbContainerSwitch_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbContainerSwitch.h" },
		{ "ModuleRelativePath", "GisbContainerSwitch.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ParameterName_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbContainerSwitch.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ParameterValue_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbContainerSwitch.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SwitchMap_ValueProp_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbContainerSwitch.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SwitchMap_MetaData[] = {
		{ "Category", "GISB" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "GisbContainerSwitch.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FNamePropertyParams NewProp_ParameterName;
	static const UECodeGen_Private::FNamePropertyParams NewProp_ParameterValue;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SwitchMap_ValueProp;
	static const UECodeGen_Private::FStrPropertyParams NewProp_SwitchMap_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_SwitchMap;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbContainerSwitch>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UGisbContainerSwitch_Statics::NewProp_ParameterName = { "ParameterName", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerSwitch, ParameterName), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ParameterName_MetaData), NewProp_ParameterName_MetaData) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UClass_UGisbContainerSwitch_Statics::NewProp_ParameterValue = { "ParameterValue", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerSwitch, ParameterValue), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ParameterValue_MetaData), NewProp_ParameterValue_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UGisbContainerSwitch_Statics::NewProp_SwitchMap_ValueProp = { "SwitchMap", nullptr, (EPropertyFlags)0x0002000000080009, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, Z_Construct_UClass_UGisbContainerBase_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SwitchMap_ValueProp_MetaData), NewProp_SwitchMap_ValueProp_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UGisbContainerSwitch_Statics::NewProp_SwitchMap_Key_KeyProp = { "SwitchMap_Key", nullptr, (EPropertyFlags)0x0002000000080009, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UClass_UGisbContainerSwitch_Statics::NewProp_SwitchMap = { "SwitchMap", nullptr, (EPropertyFlags)0x001000800000000d, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerSwitch, SwitchMap), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SwitchMap_MetaData), NewProp_SwitchMap_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbContainerSwitch_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerSwitch_Statics::NewProp_ParameterName,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerSwitch_Statics::NewProp_ParameterValue,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerSwitch_Statics::NewProp_SwitchMap_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerSwitch_Statics::NewProp_SwitchMap_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerSwitch_Statics::NewProp_SwitchMap,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerSwitch_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbContainerSwitch_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGisbContainerBase,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerSwitch_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbContainerSwitch_Statics::ClassParams = {
	&UGisbContainerSwitch::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbContainerSwitch_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerSwitch_Statics::PropPointers),
	0,
	0x00A010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerSwitch_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbContainerSwitch_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbContainerSwitch()
{
	if (!Z_Registration_Info_UClass_UGisbContainerSwitch.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbContainerSwitch.OuterSingleton, Z_Construct_UClass_UGisbContainerSwitch_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbContainerSwitch.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbContainerSwitch>()
{
	return UGisbContainerSwitch::StaticClass();
}
UGisbContainerSwitch::UGisbContainerSwitch(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbContainerSwitch);
UGisbContainerSwitch::~UGisbContainerSwitch() {}
// End Class UGisbContainerSwitch

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerSwitch_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbContainerSwitch, UGisbContainerSwitch::StaticClass, TEXT("UGisbContainerSwitch"), &Z_Registration_Info_UClass_UGisbContainerSwitch, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbContainerSwitch), 1254488875U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerSwitch_h_1846991369(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerSwitch_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerSwitch_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
