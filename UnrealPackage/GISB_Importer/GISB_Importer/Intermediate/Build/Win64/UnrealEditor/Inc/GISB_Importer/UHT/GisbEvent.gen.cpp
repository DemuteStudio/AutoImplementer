// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbEvent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbEvent() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerBase_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbEvent();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbEvent_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbEvent
void UGisbEvent::StaticRegisterNativesUGisbEvent()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbEvent);
UClass* Z_Construct_UClass_UGisbEvent_NoRegister()
{
	return UGisbEvent::StaticClass();
}
struct Z_Construct_UClass_UGisbEvent_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "DisplayName", "Single Item" },
		{ "IncludePath", "GisbEvent.h" },
		{ "ModuleRelativePath", "GisbEvent.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ContainerClass_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbEvent.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FSoftClassPropertyParams NewProp_ContainerClass;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbEvent>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FSoftClassPropertyParams Z_Construct_UClass_UGisbEvent_Statics::NewProp_ContainerClass = { "ContainerClass", nullptr, (EPropertyFlags)0x0014000000000005, UECodeGen_Private::EPropertyGenFlags::SoftClass, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbEvent, ContainerClass), Z_Construct_UClass_UGisbContainerBase_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ContainerClass_MetaData), NewProp_ContainerClass_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbEvent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbEvent_Statics::NewProp_ContainerClass,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbEvent_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbEvent_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbEvent_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbEvent_Statics::ClassParams = {
	&UGisbEvent::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbEvent_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbEvent_Statics::PropPointers),
	0,
	0x002030A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbEvent_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbEvent_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbEvent()
{
	if (!Z_Registration_Info_UClass_UGisbEvent.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbEvent.OuterSingleton, Z_Construct_UClass_UGisbEvent_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbEvent.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbEvent>()
{
	return UGisbEvent::StaticClass();
}
UGisbEvent::UGisbEvent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbEvent);
UGisbEvent::~UGisbEvent() {}
// End Class UGisbEvent

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbEvent_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbEvent, UGisbEvent::StaticClass, TEXT("UGisbEvent"), &Z_Registration_Info_UClass_UGisbEvent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbEvent), 3896466103U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbEvent_h_602464999(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbEvent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbEvent_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
