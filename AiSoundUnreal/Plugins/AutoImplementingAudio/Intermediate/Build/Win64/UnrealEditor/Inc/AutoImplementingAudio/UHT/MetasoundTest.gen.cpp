// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AutoImplementingAudio/Public/MetasoundTest.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMetasoundTest() {}

// Begin Cross Module References
AUTOIMPLEMENTINGAUDIO_API UClass* Z_Construct_UClass_UMetasoundTest();
AUTOIMPLEMENTINGAUDIO_API UClass* Z_Construct_UClass_UMetasoundTest_NoRegister();
METASOUNDENGINE_API UClass* Z_Construct_UClass_UMetaSoundSource();
UPackage* Z_Construct_UPackage__Script_AutoImplementingAudio();
// End Cross Module References

// Begin Class UMetasoundTest Function GetSound
static FName NAME_UMetasoundTest_GetSound = FName(TEXT("GetSound"));
void UMetasoundTest::GetSound()
{
	ProcessEvent(FindFunctionChecked(NAME_UMetasoundTest_GetSound),NULL);
}
struct Z_Construct_UFunction_UMetasoundTest_GetSound_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/MetasoundTest.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UMetasoundTest_GetSound_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UMetasoundTest, nullptr, "GetSound", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08020C00, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UMetasoundTest_GetSound_Statics::Function_MetaDataParams), Z_Construct_UFunction_UMetasoundTest_GetSound_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UMetasoundTest_GetSound()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UMetasoundTest_GetSound_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UMetasoundTest::execGetSound)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->GetSound_Implementation();
	P_NATIVE_END;
}
// End Class UMetasoundTest Function GetSound

// Begin Class UMetasoundTest
void UMetasoundTest::StaticRegisterNativesUMetasoundTest()
{
	UClass* Class = UMetasoundTest::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "GetSound", &UMetasoundTest::execGetSound },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UMetasoundTest);
UClass* Z_Construct_UClass_UMetasoundTest_NoRegister()
{
	return UMetasoundTest::StaticClass();
}
struct Z_Construct_UClass_UMetasoundTest_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "HideCategories", "object Object Object" },
		{ "IncludePath", "MetasoundTest.h" },
		{ "ModuleRelativePath", "Public/MetasoundTest.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UMetasoundTest_GetSound, "GetSound" }, // 2624779684
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UMetasoundTest>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UMetasoundTest_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UMetaSoundSource,
	(UObject* (*)())Z_Construct_UPackage__Script_AutoImplementingAudio,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UMetasoundTest_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UMetasoundTest_Statics::ClassParams = {
	&UMetasoundTest::StaticClass,
	"Metasound",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x009010A6u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UMetasoundTest_Statics::Class_MetaDataParams), Z_Construct_UClass_UMetasoundTest_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UMetasoundTest()
{
	if (!Z_Registration_Info_UClass_UMetasoundTest.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UMetasoundTest.OuterSingleton, Z_Construct_UClass_UMetasoundTest_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UMetasoundTest.OuterSingleton;
}
template<> AUTOIMPLEMENTINGAUDIO_API UClass* StaticClass<UMetasoundTest>()
{
	return UMetasoundTest::StaticClass();
}
UMetasoundTest::UMetasoundTest(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UMetasoundTest);
UMetasoundTest::~UMetasoundTest() {}
// End Class UMetasoundTest

// Begin Registration
struct Z_CompiledInDeferFile_FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UMetasoundTest, UMetasoundTest::StaticClass, TEXT("UMetasoundTest"), &Z_Registration_Info_UClass_UMetasoundTest, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UMetasoundTest), 317823477U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_4067415328(TEXT("/Script/AutoImplementingAudio"),
	Z_CompiledInDeferFile_FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
