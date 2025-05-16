// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbImportContainerSimpleSound.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbImportContainerSimpleSound() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_USoundWave_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBase();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerSimpleSound();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerSimpleSound_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbImportContainerSimpleSound
void UGisbImportContainerSimpleSound::StaticRegisterNativesUGisbImportContainerSimpleSound()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbImportContainerSimpleSound);
UClass* Z_Construct_UClass_UGisbImportContainerSimpleSound_NoRegister()
{
	return UGisbImportContainerSimpleSound::StaticClass();
}
struct Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbImportContainerSimpleSound.h" },
		{ "ModuleRelativePath", "GisbImportContainerSimpleSound.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_AudioFilePath_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbImportContainerSimpleSound.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_loop_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbImportContainerSimpleSound.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_SoundWave_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbImportContainerSimpleSound.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_AudioFilePath;
	static void NewProp_loop_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_loop;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SoundWave;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbImportContainerSimpleSound>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::NewProp_AudioFilePath = { "AudioFilePath", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerSimpleSound, AudioFilePath), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_AudioFilePath_MetaData), NewProp_AudioFilePath_MetaData) };
void Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::NewProp_loop_SetBit(void* Obj)
{
	((UGisbImportContainerSimpleSound*)Obj)->loop = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::NewProp_loop = { "loop", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UGisbImportContainerSimpleSound), &Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::NewProp_loop_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_loop_MetaData), NewProp_loop_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::NewProp_SoundWave = { "SoundWave", nullptr, (EPropertyFlags)0x0010000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbImportContainerSimpleSound, SoundWave), Z_Construct_UClass_USoundWave_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_SoundWave_MetaData), NewProp_SoundWave_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::NewProp_AudioFilePath,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::NewProp_loop,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::NewProp_SoundWave,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGisbImportContainerBase,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::ClassParams = {
	&UGisbImportContainerSimpleSound::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::PropPointers),
	0,
	0x000010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbImportContainerSimpleSound()
{
	if (!Z_Registration_Info_UClass_UGisbImportContainerSimpleSound.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbImportContainerSimpleSound.OuterSingleton, Z_Construct_UClass_UGisbImportContainerSimpleSound_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbImportContainerSimpleSound.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbImportContainerSimpleSound>()
{
	return UGisbImportContainerSimpleSound::StaticClass();
}
UGisbImportContainerSimpleSound::UGisbImportContainerSimpleSound(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbImportContainerSimpleSound);
UGisbImportContainerSimpleSound::~UGisbImportContainerSimpleSound() {}
// End Class UGisbImportContainerSimpleSound

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerSimpleSound_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbImportContainerSimpleSound, UGisbImportContainerSimpleSound::StaticClass, TEXT("UGisbImportContainerSimpleSound"), &Z_Registration_Info_UClass_UGisbImportContainerSimpleSound, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbImportContainerSimpleSound), 3803163161U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerSimpleSound_h_1927706988(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerSimpleSound_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbImportContainerSimpleSound_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
