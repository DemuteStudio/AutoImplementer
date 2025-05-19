// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbContainerSimpleSound.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbContainerSimpleSound() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_USoundBase_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerBase();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerSimpleSound();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbContainerSimpleSound_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbContainerSimpleSound
void UGisbContainerSimpleSound::StaticRegisterNativesUGisbContainerSimpleSound()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbContainerSimpleSound);
UClass* Z_Construct_UClass_UGisbContainerSimpleSound_NoRegister()
{
	return UGisbContainerSimpleSound::StaticClass();
}
struct Z_Construct_UClass_UGisbContainerSimpleSound_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GisbContainerSimpleSound.h" },
		{ "ModuleRelativePath", "GisbContainerSimpleSound.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Sound_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbContainerSimpleSound.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_loop_MetaData[] = {
		{ "Category", "GISB" },
		{ "ModuleRelativePath", "GisbContainerSimpleSound.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_Sound;
	static void NewProp_loop_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_loop;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbContainerSimpleSound>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UGisbContainerSimpleSound_Statics::NewProp_Sound = { "Sound", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UGisbContainerSimpleSound, Sound), Z_Construct_UClass_USoundBase_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Sound_MetaData), NewProp_Sound_MetaData) };
void Z_Construct_UClass_UGisbContainerSimpleSound_Statics::NewProp_loop_SetBit(void* Obj)
{
	((UGisbContainerSimpleSound*)Obj)->loop = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_UGisbContainerSimpleSound_Statics::NewProp_loop = { "loop", nullptr, (EPropertyFlags)0x0010000000000005, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(UGisbContainerSimpleSound), &Z_Construct_UClass_UGisbContainerSimpleSound_Statics::NewProp_loop_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_loop_MetaData), NewProp_loop_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGisbContainerSimpleSound_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerSimpleSound_Statics::NewProp_Sound,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGisbContainerSimpleSound_Statics::NewProp_loop,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerSimpleSound_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UGisbContainerSimpleSound_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGisbContainerBase,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerSimpleSound_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbContainerSimpleSound_Statics::ClassParams = {
	&UGisbContainerSimpleSound::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UGisbContainerSimpleSound_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerSimpleSound_Statics::PropPointers),
	0,
	0x002010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbContainerSimpleSound_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbContainerSimpleSound_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbContainerSimpleSound()
{
	if (!Z_Registration_Info_UClass_UGisbContainerSimpleSound.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbContainerSimpleSound.OuterSingleton, Z_Construct_UClass_UGisbContainerSimpleSound_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbContainerSimpleSound.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbContainerSimpleSound>()
{
	return UGisbContainerSimpleSound::StaticClass();
}
UGisbContainerSimpleSound::UGisbContainerSimpleSound(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbContainerSimpleSound);
UGisbContainerSimpleSound::~UGisbContainerSimpleSound() {}
// End Class UGisbContainerSimpleSound

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerSimpleSound_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbContainerSimpleSound, UGisbContainerSimpleSound::StaticClass, TEXT("UGisbContainerSimpleSound"), &Z_Registration_Info_UClass_UGisbContainerSimpleSound, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbContainerSimpleSound), 960336975U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerSimpleSound_h_3642629020(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerSimpleSound_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbContainerSimpleSound_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
