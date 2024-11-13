// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "MetasoundTest.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef AUTOIMPLEMENTINGAUDIO_MetasoundTest_generated_h
#error "MetasoundTest.generated.h already included, missing '#pragma once' in MetasoundTest.h"
#endif
#define AUTOIMPLEMENTINGAUDIO_MetasoundTest_generated_h

#define FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	virtual void GetSound_Implementation(); \
	DECLARE_FUNCTION(execGetSound);


#define FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_16_CALLBACK_WRAPPERS
#define FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_16_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUMetasoundTest(); \
	friend struct Z_Construct_UClass_UMetasoundTest_Statics; \
public: \
	DECLARE_CLASS(UMetasoundTest, UMetaSoundSource, COMPILED_IN_FLAGS(0 | CLASS_DefaultConfig | CLASS_Config), CASTCLASS_None, TEXT("/Script/AutoImplementingAudio"), NO_API) \
	DECLARE_SERIALIZER(UMetasoundTest)


#define FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_16_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UMetasoundTest(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UMetasoundTest(UMetasoundTest&&); \
	UMetasoundTest(const UMetasoundTest&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UMetasoundTest); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UMetasoundTest); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UMetasoundTest) \
	NO_API virtual ~UMetasoundTest();


#define FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_13_PROLOG
#define FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_16_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_16_CALLBACK_WRAPPERS \
	FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_16_INCLASS_NO_PURE_DECLS \
	FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h_16_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> AUTOIMPLEMENTINGAUDIO_API UClass* StaticClass<class UMetasoundTest>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_AiSoundUnreal_Plugins_AutoImplementingAudio_Source_AutoImplementingAudio_Public_MetasoundTest_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
