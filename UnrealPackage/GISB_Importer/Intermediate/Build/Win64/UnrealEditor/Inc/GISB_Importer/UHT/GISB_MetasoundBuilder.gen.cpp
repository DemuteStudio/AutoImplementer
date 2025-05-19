// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GISB_Importer/GISB_MetasoundBuilder.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGISB_MetasoundBuilder() {}

// Begin Cross Module References
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGISB_MetasoundBuilder();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGISB_MetasoundBuilder_NoRegister();
GISB_IMPORTER_API UClass* Z_Construct_UClass_UGisbImportContainerBase_NoRegister();
METASOUNDENGINE_API UClass* Z_Construct_UClass_UMetaSoundSource_NoRegister();
UPackage* Z_Construct_UPackage__Script_GISB_Importer();
// End Cross Module References

// Begin Class UGISB_MetasoundBuilder Function CreateMetasoundFromGISB
struct Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics
{
	struct GISB_MetasoundBuilder_eventCreateMetasoundFromGISB_Parms
	{
		UGisbImportContainerBase* gisb;
		FString Name;
		FString path;
		UMetaSoundSource* ReturnValue;
	};
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "Metasound" },
		{ "ModuleRelativePath", "GISB_MetasoundBuilder.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Name_MetaData[] = {
		{ "NativeConst", "" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_path_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_gisb;
	static const UECodeGen_Private::FStrPropertyParams NewProp_Name;
	static const UECodeGen_Private::FStrPropertyParams NewProp_path;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::NewProp_gisb = { "gisb", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(GISB_MetasoundBuilder_eventCreateMetasoundFromGISB_Parms, gisb), Z_Construct_UClass_UGisbImportContainerBase_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::NewProp_Name = { "Name", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(GISB_MetasoundBuilder_eventCreateMetasoundFromGISB_Parms, Name), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Name_MetaData), NewProp_Name_MetaData) };
const UECodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::NewProp_path = { "path", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(GISB_MetasoundBuilder_eventCreateMetasoundFromGISB_Parms, path), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_path_MetaData), NewProp_path_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(GISB_MetasoundBuilder_eventCreateMetasoundFromGISB_Parms, ReturnValue), Z_Construct_UClass_UMetaSoundSource_NoRegister, METADATA_PARAMS(0, nullptr) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::NewProp_gisb,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::NewProp_Name,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::NewProp_path,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::NewProp_ReturnValue,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::PropPointers) < 2048);
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGISB_MetasoundBuilder, nullptr, "CreateMetasoundFromGISB", nullptr, nullptr, Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::PropPointers), sizeof(Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::GISB_MetasoundBuilder_eventCreateMetasoundFromGISB_Parms), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::Function_MetaDataParams), Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::Function_MetaDataParams) };
static_assert(sizeof(Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::GISB_MetasoundBuilder_eventCreateMetasoundFromGISB_Parms) < MAX_uint16);
UFunction* Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UGISB_MetasoundBuilder::execCreateMetasoundFromGISB)
{
	P_GET_OBJECT(UGisbImportContainerBase,Z_Param_gisb);
	P_GET_PROPERTY(FStrProperty,Z_Param_Name);
	P_GET_PROPERTY(FStrProperty,Z_Param_path);
	P_FINISH;
	P_NATIVE_BEGIN;
	*(UMetaSoundSource**)Z_Param__Result=UGISB_MetasoundBuilder::CreateMetasoundFromGISB(Z_Param_gisb,Z_Param_Name,Z_Param_path);
	P_NATIVE_END;
}
// End Class UGISB_MetasoundBuilder Function CreateMetasoundFromGISB

// Begin Class UGISB_MetasoundBuilder
void UGISB_MetasoundBuilder::StaticRegisterNativesUGISB_MetasoundBuilder()
{
	UClass* Class = UGISB_MetasoundBuilder::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "CreateMetasoundFromGISB", &UGISB_MetasoundBuilder::execCreateMetasoundFromGISB },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGISB_MetasoundBuilder);
UClass* Z_Construct_UClass_UGISB_MetasoundBuilder_NoRegister()
{
	return UGISB_MetasoundBuilder::StaticClass();
}
struct Z_Construct_UClass_UGISB_MetasoundBuilder_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "IncludePath", "GISB_MetasoundBuilder.h" },
		{ "ModuleRelativePath", "GISB_MetasoundBuilder.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UGISB_MetasoundBuilder_CreateMetasoundFromGISB, "CreateMetasoundFromGISB" }, // 2071340584
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGISB_MetasoundBuilder>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UGISB_MetasoundBuilder_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_GISB_Importer,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UGISB_MetasoundBuilder_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UGISB_MetasoundBuilder_Statics::ClassParams = {
	&UGISB_MetasoundBuilder::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UGISB_MetasoundBuilder_Statics::Class_MetaDataParams), Z_Construct_UClass_UGISB_MetasoundBuilder_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UGISB_MetasoundBuilder()
{
	if (!Z_Registration_Info_UClass_UGISB_MetasoundBuilder.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGISB_MetasoundBuilder.OuterSingleton, Z_Construct_UClass_UGISB_MetasoundBuilder_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UGISB_MetasoundBuilder.OuterSingleton;
}
template<> GISB_IMPORTER_API UClass* StaticClass<UGISB_MetasoundBuilder>()
{
	return UGISB_MetasoundBuilder::StaticClass();
}
UGISB_MetasoundBuilder::UGISB_MetasoundBuilder(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UGISB_MetasoundBuilder);
UGISB_MetasoundBuilder::~UGISB_MetasoundBuilder() {}
// End Class UGISB_MetasoundBuilder

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GISB_MetasoundBuilder_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UGISB_MetasoundBuilder, UGISB_MetasoundBuilder::StaticClass, TEXT("UGISB_MetasoundBuilder"), &Z_Registration_Info_UClass_UGISB_MetasoundBuilder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGISB_MetasoundBuilder), 3896584916U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GISB_MetasoundBuilder_h_2923876056(TEXT("/Script/GISB_Importer"),
	Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GISB_MetasoundBuilder_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_bucqu_OneDrive_Documents_Matheo_Olympus_AutoImplementer_UnrealPackage_GISB_Importer_HostProject_Plugins_GISB_Importer_Source_GISB_Importer_GISB_MetasoundBuilder_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
