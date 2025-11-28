// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MetasoundEngine/Public/MetasoundSource.h"
#include "MetasoundTest.generated.h"

/**
 * 
 */
UCLASS()
class AUTOIMPLEMENTINGAUDIO_API UMetasoundTest : public UMetaSoundSource
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void GetSound();
};
