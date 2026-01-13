// Copyright Epic Games, Inc. All Rights Reserved.

#include "GISB_Importer.h"
#include "GISB_ImporterStyle.h"
#include "GISB_ImporterCommands.h"
#include "MetasoundFrontendModuleRegistrationMacros.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "GisbImportContainerBase.h"
#include "GISB_MetasoundSourceBuilder.h"
#include "MetasoundSource.h"
#include "Framework/Application/SlateApplication.h"

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
	METASOUND_REGISTER_ITEMS_IN_MODULE
}

void FGISB_ImporterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGISB_ImporterStyle::Shutdown();

	FGISB_ImporterCommands::Unregister();

	METASOUND_UNREGISTER_ITEMS_IN_MODULE
}

void FGISB_ImporterModule::PluginButtonClicked()
{
	// Import ISX JSON bank directly to MetaSounds (bypassing runtime container)
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get Desktop Platform"));
		return;
	}

	// Open file dialog to select JSON file
	FString FileTypes = TEXT("ISX JSON files (*.json)|*.json");
	TArray<FString> OutFiles;
	const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
	bool bFileChosen = DesktopPlatform->OpenFileDialog(
		ParentWindowHandle,
		TEXT("Select ISX Bank or Event JSON"),
		FPaths::ProjectContentDir(),
		TEXT(""),
		FileTypes,
		EFileDialogFlags::None,
		OutFiles
	);

	if (!bFileChosen || OutFiles.Num() < 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("No file selected"));
		return;
	}

	// Load JSON file
	FString JsonContent;
	if (!FFileHelper::LoadFileToString(JsonContent, *OutFiles[0]))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *OutFiles[0]);
		return;
	}

	FString FilePath = OutFiles[0];
	FString FullPath = FPaths::ConvertRelativePathToFull(FilePath);

	// Parse JSON
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);
	TSharedPtr<FJsonObject> JsonObject;

	if (!FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON file: %s"), *FilePath);
		return;
	}

	FString AssetName;
	if (!JsonObject->TryGetStringField(TEXT("name"), AssetName))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get name field from JSON"));
		return;
	}

	// MetaSound output path
	FString MSPackagePath = TEXT("/Game/ISX_SoundBanks/MetaSounds/");

	// Check if this is a single event or a bank with multiple events
	const TSharedPtr<FJsonObject>* RootAudioObject;
	TArray<FString> EventArrayJson;

	if (JsonObject->TryGetObjectField(TEXT("rootAudioObject"), RootAudioObject))
	{
		// Single event JSON
		UE_LOG(LogTemp, Log, TEXT("Importing single event: %s"), *AssetName);
		ImportEventDirectToMetaSound(AssetName, FullPath, RootAudioObject, MSPackagePath);
	}
	else if (JsonObject->TryGetStringArrayField(TEXT("events"), EventArrayJson))
	{
		// Bank JSON with multiple events
		UE_LOG(LogTemp, Log, TEXT("Importing bank: %s with %d events"), *AssetName, EventArrayJson.Num());

		for (const FString& EventEntry : EventArrayJson)
		{
			// Load event JSON from Events subfolder
			FString EventFilePath = FPaths::Combine(FPaths::GetPath(FilePath), TEXT("Events"), EventEntry + TEXT(".json"));

			FString EventJsonContent;
			if (!FFileHelper::LoadFileToString(EventJsonContent, *EventFilePath))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to load event JSON: %s"), *EventFilePath);
				continue;
			}

			TSharedRef<TJsonReader<>> EventReader = TJsonReaderFactory<>::Create(EventJsonContent);
			TSharedPtr<FJsonObject> EventJsonObject;

			if (!FJsonSerializer::Deserialize(EventReader, EventJsonObject))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to deserialize event JSON: %s"), *EventFilePath);
				continue;
			}

			FString EventName;
			if (!EventJsonObject->TryGetStringField(TEXT("name"), EventName))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to get event name from JSON: %s"), *EventFilePath);
				continue;
			}

			const TSharedPtr<FJsonObject>* EventRootAudioObject;
			if (!EventJsonObject->TryGetObjectField(TEXT("rootAudioObject"), EventRootAudioObject))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to get rootAudioObject from event: %s"), *EventName);
				continue;
			}

			ImportEventDirectToMetaSound(EventName, EventFilePath, EventRootAudioObject, MSPackagePath);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JSON is neither a valid event nor bank (missing rootAudioObject or events field)"));
		return;
	}

	// Show success message
	FText DialogText = FText::Format(
		LOCTEXT("ImportSuccessText", "Successfully imported ISX bank/event: {0}\nMetaSounds created at: {1}"),
		FText::FromString(AssetName),
		FText::FromString(MSPackagePath)
	);
	FMessageDialog::Open(EAppMsgCategory::Info, EAppMsgType::Ok, DialogText);
}

void FGISB_ImporterModule::ImportEventDirectToMetaSound(
	const FString& EventName,
	const FString& EventFilePath,
	const TSharedPtr<FJsonObject>* RootAudioObject,
	const FString& MSPackagePath)
{
	UE_LOG(LogTemp, Log, TEXT("Importing event to MetaSound: %s"), *EventName);

	// Create ImportContainer from JSON (no outer package needed for direct MetaSound creation)
	UGisbImportContainerBase* ImportContainer = UGisbImportContainerBase::CreateFromJson(
		*RootAudioObject,
		GetTransientPackage(),  // Use transient package since we don't need persistent ImportContainer
		EventFilePath
	);

	if (!ImportContainer)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create ImportContainer for event: %s"), *EventName);
		return;
	}

	// Create MetaSound directly from ImportContainer (bypassing RuntimeContainer)
	UMetaSoundSource* GeneratedMS = UGISB_MetasoundSourceBuilder::CreateMetasoundFromGISB(
		ImportContainer,
		EventName,
		MSPackagePath
	);

	if (!GeneratedMS)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create MetaSound for event: %s"), *EventName);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Successfully created MetaSound: %s at %s"), *EventName, *MSPackagePath);
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
METASOUND_IMPLEMENT_MODULE_REGISTRATION_LIST
IMPLEMENT_MODULE(FGISB_ImporterModule, GISB_Importer)