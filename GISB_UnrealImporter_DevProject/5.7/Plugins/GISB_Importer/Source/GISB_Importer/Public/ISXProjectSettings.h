#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "ISXProjectSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="ISX Settings"))
class GISB_IMPORTER_API UISXProjectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UISXProjectSettings();

	// Distance scale multiplier for converting Unity units to Unreal units
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Import",
		meta=(ClampMin="0.01", UIMin="1.0", UIMax="10000.0",
		ToolTip="Multiplier for distance values when importing attenuation from JSON. Unity to Unreal scale conversion (default: 1000)."))
	float DistanceScale;
};
