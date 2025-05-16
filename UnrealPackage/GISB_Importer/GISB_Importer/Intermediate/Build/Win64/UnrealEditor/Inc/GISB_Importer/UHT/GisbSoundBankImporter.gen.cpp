// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbSoundBankImporter.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbSoundBankImporter() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbSoundBankImporter();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbSoundBankImporter_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbSoundBankImporter Function ImportSoundBankFromJson
struct Z_Construct_UFunction_UGisbSoundBankImporter_ImportSoundBankFromJson_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "SoundBank" },
		{ "ModuleRelativePath", "GisbSoundBankImporter.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UGisbSoundBankImporter_ImportSoundBankFromJson_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGisbSoundBankImporter, nullptr, "ImportSoundBankFromJson", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UGisbSoundBankImporter_ImportSoundBankFromJson_Statics::Function_MetaDataParams), Z_Construct_UFunction_UGisbSoundBankImporter_ImportSoundBankFromJson_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UGisbSoundBankImporter_ImportSoundBankFromJson()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UGisbSoundBankImporter_ImportSoundBankFromJson_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UGisbSoundBankImporter::execImportSoundBankFromJson)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	UGisbSoundBankImporter::ImportSoundBankFromJson();
	P_NATIVE_END;
}
// End Class UGisbSoundBankImporter Function ImportSoundBankFromJson

// Begin Class UGisbSoundBankImporter
void UGisbSoundBankImporter::StaticRegisterNativesUGisbSoundBankImporter()
{
	UClass* Class = UGisbSoundBankImporter::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "ImportSoundBankFromJson", &UGisbSoundBankImporter::execImportSoundBankFromJson },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbSoundBankImporter);
UClass* Z_Construct_UClass_UGisbSoundBankImporter_NoRegister()
{
	return UGisbSoundBankImporter::StaticClass();
}
struct Z_Construct_UClass_UGisbSoundBankImporter_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbSoundBankImporter.h" },
		{ "ModuleRelativePath", "GisbSoundBankImporter.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UGisbSoundBankImporter_ImportSoundBankFromJson, "ImportSoundBankFromJson" }, // 1799814928
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbSoundBankImporter>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UGisbSoundBankImporter_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbSoundBankImporter_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbSoundBankImporter_Statics::ClassParams = {
	&UGisbSoundBankImporter::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x000000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbSoundBankImporter_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbSoundBankImporter_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbSoundBankImporter()
{
	if (!Z_Registration_Info_UClass_UGisbSoundBankImporter.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbSoundBankImporter.OuterSingleton, Z_Construct_UClass_UGisbSoundBankImporter_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbSoundBankImporter.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbSoundBankImporter>()
{
	return UGisbSoundBankImporter::StaticClass();
}
UGisbSoundBankImporter::UGisbSoundBankImporter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbSoundBankImporter);
UGisbSoundBankImporter::~UGisbSoundBankImporter() {}
// End Class UGisbSoundBankImporter

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankImporter_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbSoundBankImporter, UGisbSoundBankImporter::StaticClass, TEXT("UGisbSoundBankImporter"), &Z_Registration_Info_UClass_UGisbSoundBankImporter, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbSoundBankImporter), 1259296554U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankImporter_h_4133913999(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankImporter_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbSoundBankImporter_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
