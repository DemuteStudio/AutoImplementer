// Copyright Epic Games, Inc. All Rights Reserved.

#include "GISB_Importer.h"
#include "GISB_ImporterStyle.h"
#include "GISB_ImporterCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName GISB_ImporterTabName("GISB_Importer");

#define LOCTEXT_NAMESPACE "FGISB_ImporterModule"

void FGISB_ImporterModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGISB_ImporterStyle::Initialize();
	FGISB_ImporterStyle::ReloadTextures();

	FGISB_ImporterCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGISB_ImporterCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FGISB_ImporterModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGISB_ImporterModule::RegisterMenus));
}

void FGISB_ImporterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGISB_ImporterStyle::Shutdown();

	FGISB_ImporterCommands::Unregister();
}

void FGISB_ImporterModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	FText DialogText = FText::Format(
							LOCTEXT("PluginButtonDialogText", "Add code to {0} in {1} to override this button's actions"),
							FText::FromString(TEXT("FGISB_ImporterModule::PluginButtonClicked()")),
							FText::FromString(TEXT("GISB_Importer.cpp"))
					   );
	FMessageDialog::Open(EAppMsgType::Ok, DialogText);
}

void FGISB_ImporterModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FGISB_ImporterCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGISB_ImporterCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGISB_ImporterModule, GISB_Importer)