// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"
#include "Templates/SharedPointer.h"

class FToolBarBuilder;
class FMenuBuilder;
class FJsonObject;

class FGISB_ImporterModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	/** Helper function to import a single event directly to MetaSound */
	void ImportEventDirectToMetaSound(
		const FString& EventName,
		const FString& EventFilePath,
		const TSharedPtr<FJsonObject>* RootAudioObject,
		const FString& MSPackagePath
	);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};
