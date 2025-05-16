// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GisbPlaybackSystem.h"
#include "Runtime/Engine/Classes/Engine/GameInstance.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGisbPlaybackSystem() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UAudioComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UGameInstanceSubsystem();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbPlaybackSystem();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbPlaybackSystem_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbSoundBankDataAsset_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGisbPlaybackSystem Function PlaySoundEvent
struct Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics
{
	struct GisbPlaybackSystem_eventPlaySoundEvent_Parms
	{
		UObject* WorldContextObject;
		UGisbSoundBankDataAsset* SoundBank;
		TMap<FName,FString> Parameters;
		UAudioComponent* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Gisb Audio" },
		{ "Comment", "/** Plays a sound event from a data asset and returns the audio component playing the sound */" },
		{ "ModuleRelativePath", "GisbPlaybackSystem.h" },
		{ "ToolTip", "Plays a sound event from a data asset and returns the audio component playing the sound" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Parameters_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_ReturnValue_MetaData[] = {
		{ "EditInline", "true" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_WorldContextObject;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_SoundBank;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Parameters_ValueProp;
	static const UECodeGen_Private::FNamePropertyParams NewProp_Parameters_Key_KeyProp;
	static const UECodeGen_Private::FMapPropertyParams NewProp_Parameters;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_WorldContextObject = { "WorldContextObject", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(GisbPlaybackSystem_eventPlaySoundEvent_Parms, WorldContextObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_SoundBank = { "SoundBank", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(GisbPlaybackSystem_eventPlaySoundEvent_Parms, SoundBank), Z_Construct_UClass_UGisbSoundBankDataAsset_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_Parameters_ValueProp = { "Parameters", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 1, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FNamePropertyParams Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_Parameters_Key_KeyProp = { "Parameters_Key", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Name, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, 0, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FMapPropertyParams Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_Parameters = { "Parameters", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Map, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(GisbPlaybackSystem_eventPlaySoundEvent_Parms, Parameters), EMapPropertyFlags::None, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Parameters_MetaData), NewProp_Parameters_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000080588, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(GisbPlaybackSystem_eventPlaySoundEvent_Parms, ReturnValue), Z_Construct_UClass_UAudioComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_ReturnValue_MetaData), NewProp_ReturnValue_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_WorldContextObject,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_SoundBank,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_Parameters_ValueProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_Parameters_Key_KeyProp,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_Parameters,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGisbPlaybackSystem, nullptr, "PlaySoundEvent", nullptr, nullptr, Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::PropPointers), sizeof(Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::GisbPlaybackSystem_eventPlaySoundEvent_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04422401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::Function_MetaDataParams), Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::GisbPlaybackSystem_eventPlaySoundEvent_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UGisbPlaybackSystem::execPlaySoundEvent)
{
	P_GET_OBJECT(UObject,Z_Param_WorldContextObject);
	P_GET_OBJECT(UGisbSoundBankDataAsset,Z_Param_SoundBank);
	P_GET_TMAP_REF(FName,FString,Z_Param_Out_Parameters);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UAudioComponent**)Z_Param__Result=UGisbPlaybackSystem::PlaySoundEvent(Z_Param_WorldContextObject,Z_Param_SoundBank,Z_Param_Out_Parameters);
	P_NATIVE_END;
}
// End Class UGisbPlaybackSystem Function PlaySoundEvent

// Begin Class UGisbPlaybackSystem
void UGisbPlaybackSystem::StaticRegisterNativesUGisbPlaybackSystem()
{
	UClass* Class = UGisbPlaybackSystem::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "PlaySoundEvent", &UGisbPlaybackSystem::execPlaySoundEvent },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGisbPlaybackSystem);
UClass* Z_Construct_UClass_UGisbPlaybackSystem_NoRegister()
{
	return UGisbPlaybackSystem::StaticClass();
}
struct Z_Construct_UClass_UGisbPlaybackSystem_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "GisbPlaybackSystem.h" },
		{ "ModuleRelativePath", "GisbPlaybackSystem.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UGisbPlaybackSystem_PlaySoundEvent, "PlaySoundEvent" }, // 683230130
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGisbPlaybackSystem>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UGisbPlaybackSystem_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGameInstanceSubsystem,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbPlaybackSystem_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGisbPlaybackSystem_Statics::ClassParams = {
	&UGisbPlaybackSystem::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGisbPlaybackSystem_Statics::Class_MetaDataParams), Z_Construct_UClass_UGisbPlaybackSystem_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGisbPlaybackSystem()
{
	if (!Z_Registration_Info_UClass_UGisbPlaybackSystem.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGisbPlaybackSystem.OuterSingleton, Z_Construct_UClass_UGisbPlaybackSystem_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGisbPlaybackSystem.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGisbPlaybackSystem>()
{
	return UGisbPlaybackSystem::StaticClass();
}
UGisbPlaybackSystem::UGisbPlaybackSystem() {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGisbPlaybackSystem);
UGisbPlaybackSystem::~UGisbPlaybackSystem() {}
// End Class UGisbPlaybackSystem

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbPlaybackSystem_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGisbPlaybackSystem, UGisbPlaybackSystem::StaticClass, TEXT("UGisbPlaybackSystem"), &Z_Registration_Info_UClass_UGisbPlaybackSystem, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGisbPlaybackSystem), 4278221973U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbPlaybackSystem_h_4185021137(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbPlaybackSystem_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GisbPlaybackSystem_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
