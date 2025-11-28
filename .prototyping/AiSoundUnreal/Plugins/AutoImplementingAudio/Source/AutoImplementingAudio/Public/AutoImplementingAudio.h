// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"

class FAutoImplementingAudioModule : public IModuleInterface
{
public:

	// This function will be called on plugin startup
	virtual void StartupModule() override;
	
	// This function will be called on plugin shutdown
	virtual void ShutdownModule() override;
	
private:
	// This function registers the menus and tool buttons
	static void RegisterMenus();

	// This function is called when the button is clicked in the Tools menu
	static void YourButtonClicked();
	
};
