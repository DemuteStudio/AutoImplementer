// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbSoundBankImporter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "GisbSoundBankDataAsset.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Misc/FileHelper.h"
#include "GisbImportContainerBase.h"
#include "Misc/Paths.h"
#include "UObject/Package.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "MetasoundSource.h"

void UGisbSoundBankImporter::ImportSoundBankFromJson()
{
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (!DesktopPlatform) return;

    FString FileTypes = TEXT("JSON files (*.json)|*.json");
    TArray<FString> OutFiles;

    const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
    bool bFileChosen = DesktopPlatform->OpenFileDialog(ParentWindowHandle, TEXT("Select JSON"), FPaths::ProjectContentDir(), TEXT(""), FileTypes, EFileDialogFlags::None, OutFiles);

    if (bFileChosen && OutFiles.Num() > 0)
    {
        FString JsonContent;
        if (FFileHelper::LoadFileToString(JsonContent, *OutFiles[0]))
        {
            FString FilePath = OutFiles[0];
            FString BaseDir = FPaths::ProjectContentDir();
            FString FullPath = /*FPaths::Combine(BaseDir, FilePath)*/ FilePath;
            FullPath = FPaths::ConvertRelativePathToFull(FullPath);
            
            // Parse JSON
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);
            TSharedPtr<FJsonObject> JsonObject;

            if (FJsonSerializer::Deserialize(Reader, JsonObject))
            {
                FString AssetName;
                if (!JsonObject->TryGetStringField("name", AssetName))
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to get event name from JSON."));
                    return;
                }

                const TSharedPtr<FJsonObject>* RootAudioObject; // Used when user selected an audio event
                TArray<FString> EventArrayJson; // Used when user selected an audio bank

                if (JsonObject->TryGetObjectField("rootAudioObject", RootAudioObject))
                {
                    ImportEventFromJson(AssetName, FullPath, RootAudioObject);
                }

                // WIP
                else if (JsonObject->TryGetStringArrayField("events", EventArrayJson ))
                {
                    for (const FString& EventEntry : EventArrayJson) 
                    {
                        FString EventFilePath = FPaths::Combine(FPaths::GetPath(FilePath), TEXT("Events"), (EventEntry + TEXT(".json")));
                        
                        FString EventJsonContent;
                        if (FFileHelper::LoadFileToString(EventJsonContent, *EventFilePath))
                        {
                            TSharedRef<TJsonReader<>> EventReader = TJsonReaderFactory<>::Create(EventJsonContent);
                            TSharedPtr<FJsonObject> EventJsonObject;
                            if (FJsonSerializer::Deserialize(EventReader, EventJsonObject))
                            {
                                FString EventName;
                                if (!EventJsonObject->TryGetStringField("name", EventName))
                                {
                                    UE_LOG(LogTemp, Error, TEXT("Failed to get event name from JSON."));
                                    return;
                                }
                                const TSharedPtr<FJsonObject>* EventRootAudioObject;
                                if (EventJsonObject->TryGetObjectField("rootAudioObject", EventRootAudioObject))
                                {
                                    ImportEventFromJson(EventName, EventFilePath, EventRootAudioObject);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void UGisbSoundBankImporter::ImportEventFromJson(FString EventName, FString FullPath,const TSharedPtr<FJsonObject>* RootAudioObject)
{
    FString AssetName = EventName + "_SoundBank";
    FString PackagePath = "/Game/SoundBanks/" + AssetName;
	FString MSPackagePath = "/Game/SoundBanks/MetaSounds/" + AssetName;
    UPackage* Package = CreatePackage(*PackagePath);
	UPackage* MSPackage = CreatePackage(*MSPackagePath);

    // Create a new sound asset and set its properties
    UGisbSoundBankDataAsset* NewAsset = NewObject<UGisbSoundBankDataAsset>(Package, *AssetName, RF_Public | RF_Standalone);
    UGisbImportContainerBase* RootContainer = UGisbImportContainerBase::CreateFromJson(*RootAudioObject, NewAsset, FullPath);
	UMetaSoundSource* GeneratedMS = NewObject<UMetaSoundSource>(MSPackage, *AssetName, RF_Public | RF_Standalone);
    if (RootContainer && GeneratedMS)
    {
        NewAsset->EventName = EventName;
        NewAsset->RootContainer = RootContainer->ToRuntimeContainer(NewAsset);
        RootContainer->MarkPackageDirty();

        GeneratedMS->SetDocument(RootContainer->ToMSDocument(AssetName));
		NewAsset->GeneratedMS = GeneratedMS;

        NewAsset->Modify();  // Ensures Unreal tracks the change
		GeneratedMS->Modify();
        NewAsset->MarkPackageDirty();
		GeneratedMS->MarkPackageDirty();
        FAssetRegistryModule::AssetCreated(NewAsset);
		FAssetRegistryModule::AssetCreated(GeneratedMS);

        // Make sure the package is marked dirty for saving
        Package->MarkPackageDirty();
		MSPackage->MarkPackageDirty();

        // Save the asset
        const FString AssetFileName = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
        bool bSaved = UPackage::SavePackage(Package, NewAsset, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *AssetFileName);
		const FString AssetMSFileName = FPackageName::LongPackageNameToFilename(MSPackagePath, FPackageName::GetAssetPackageExtension());
		bool bSavedMS = UPackage::SavePackage(MSPackage, GeneratedMS, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *AssetMSFileName);

        if (bSaved)
        {
            UE_LOG(LogTemp, Log, TEXT("Asset successfully saved: %s"), *AssetFileName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save the asset: %s"), *AssetFileName);
        }

        if (bSavedMS)
        {
            UE_LOG(LogTemp, Log, TEXT("MetaSound successfully saved: %s"), *AssetMSFileName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save the MetaSound: %s"), *AssetMSFileName);
        }
    }
}