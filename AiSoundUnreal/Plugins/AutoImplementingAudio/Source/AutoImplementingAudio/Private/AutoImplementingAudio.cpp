// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutoImplementingAudio.h"
#include "LevelEditor.h"
#include "ToolMenus.h"

#define LOCTEXT_NAMESPACE "FAutoImplementingAudioModule"

void FAutoImplementingAudioModule::StartupModule()
{
	// Register the tool menu
	if (UToolMenus::IsToolMenuUIEnabled())
	{
		UToolMenus::Get()->RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateStatic(&FAutoImplementingAudioModule::RegisterMenus));
	}
	
}

void FAutoImplementingAudioModule::ShutdownModule()
{
	// Unregister menus or anything else initialized in StartupModule
	UToolMenus::UnregisterOwner(this);
}

// ---------------- Toolbar integration ---------------//
void FAutoImplementingAudioModule::RegisterMenus()
{
	// Extend the "Tools" menu
	UToolMenu* ToolsMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Tools");

	FToolMenuSection& Section = ToolsMenu->AddSection("DemuteTools", FText::FromString("Demute"));
	Section.AddMenuEntry(
		"AiSoundTool",
		FText::FromString("AiSoundTool"),
		FText::FromString("Parses a Json to a Metasound"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.ToolIcon"),  // Replace with your custom icon if available
		FUIAction(FExecuteAction::CreateStatic(&FAutoImplementingAudioModule::YourButtonClicked))
	);
}

void FAutoImplementingAudioModule::YourButtonClicked()
{
	// Code to execute when the tool is clicked
	UE_LOG(LogTemp, Log, TEXT("Your Tool clicked from Tools menu!"));
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAutoImplementingAudioModule, AutoImplementingAudio)