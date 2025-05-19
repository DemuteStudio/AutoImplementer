// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbSoundBankParser.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbSoundBankParser() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbSoundBankParser();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbSoundBankParser_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbSoundBankParser
void UGisbSoundBankParser::StaticRegisterNativesUGisbSoundBankParser()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbSoundBankParser);
UClass* Z_Construct_UClass_UGisbSoundBankParser_NoRegister()
{
	return UGisbSoundBankParser::StaticClass();
}
struct Z_Construct_UClass_UGisbSoundBankParser_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbSoundBankParser.h" },
		{ "ModuleRelativePath", "GisbSoundBankParser.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbSoundBankParser>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UGisbSoundBankParser_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbSoundBankParser_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbSoundBankParser_Statics::ClassParams = {
	&UGisbSoundBankParser::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbSoundBankParser_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbSoundBankParser_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbSoundBankParser()
{
	if (!Z_Registration_Info_UClass_UGisbSoundBankParser.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbSoundBankParser.OuterSingleton, Z_Construct_UClass_UGisbSoundBankParser_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbSoundBankParser.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbSoundBankParser>()
{
	return UGisbSoundBankParser::StaticClass();
}
UGisbSoundBankParser::UGisbSoundBankParser(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbSoundBankParser);
UGisbSoundBankParser::~UGisbSoundBankParser() {}
// End Class UGisbSoundBankParser

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankParser_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbSoundBankParser, UGisbSoundBankParser::StaticClass, TEXT("UGisbSoundBankParser"), &Z_Registration_Info_UClass_UGisbSoundBankParser, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbSoundBankParser), 771056571U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankParser_h_3299127264(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankParser_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankParser_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
