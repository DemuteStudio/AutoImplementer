// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbSoundBankImporter.h"
#include "Dom/JsonObject.h"
#include "Containers/StringFwd.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "GisbSoundBankDataAsset.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Misc/FileHelper.h"
#include "GisbImportContainerBase.h"
#include "Misc/Paths.h"
#include "UObject/Package.h"
#include "UObject/SavePackage.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "GISB_MetasoundBuilder.h"
#include "MetasoundSource.h"
#include "EditorAssetLibrary.h"

void UGisbSoundBankImporter::ImportSoundBankFromJson()
{
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if (!DesktopPlatform) return;

    FString FileTypes = TEXT("JSON files (*.json)|*.json");
    TArray<FString> OutFiles;

    const void* ParentWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
    bool bFileChosen = DesktopPlatform->OpenFileDialog(ParentWindowHandle, TEXT("Select JSON"), FPaths::ProjectContentDir(), TEXT(""), FileTypes, EFileDialogFlags::None, OutFiles);

    if (!bFileChosen || OutFiles.Num() < 1) return;
    
    FString JsonContent;
    if (!FFileHelper::LoadFileToString(JsonContent, *OutFiles[0])) return;
        
    FString FilePath = OutFiles[0];
    FString BaseDir = FPaths::ProjectContentDir();
    FString FullPath = FilePath;
    FullPath = FPaths::ConvertRelativePathToFull(FullPath);
            
    // Parse JSON
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);
    TSharedPtr<FJsonObject> JsonObject;

    if (!FJsonSerializer::Deserialize(Reader, JsonObject)) return;
            
    FString AssetName;
    if (!JsonObject->TryGetStringField(TEXT("name"), AssetName))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get event name from JSON."));
        return;
    }

    const TSharedPtr<FJsonObject>* RootAudioObject; // Used when user selected an audio event
    TArray<FString> EventArrayJson; // Used when user selected an audio bank

    if (JsonObject->TryGetObjectField(TEXT("rootAudioObject"), RootAudioObject))
    {
        ImportEventFromJson(AssetName, FullPath, RootAudioObject);
    }
    else if (JsonObject->TryGetStringArrayField(TEXT("events"), EventArrayJson ))
    {
        for (const FString& EventEntry : EventArrayJson) 
        {
            FString EventFilePath = FPaths::Combine(FPaths::GetPath(FilePath), TEXT("Events"), (EventEntry + TEXT(".json")));
                        
            FString EventJsonContent;

            if (!FFileHelper::LoadFileToString(EventJsonContent, *EventFilePath)) return;
                        
            TSharedRef<TJsonReader<>> EventReader = TJsonReaderFactory<>::Create(EventJsonContent);
            TSharedPtr<FJsonObject> EventJsonObject;

            if (!FJsonSerializer::Deserialize(EventReader, EventJsonObject)) return;
            
            FString EventName;
            if (!EventJsonObject->TryGetStringField(TEXT("name"), EventName))
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to get event name from JSON."));
                return;
            }
            const TSharedPtr<FJsonObject>* EventRootAudioObject;

            if (!EventJsonObject->TryGetObjectField(TEXT("rootAudioObject"), EventRootAudioObject)) return;
            ImportEventFromJson(EventName, EventFilePath, EventRootAudioObject);
        }
    }
}

void UGisbSoundBankImporter::ImportEventFromJson(FString EventName, FString FullPath,const TSharedPtr<FJsonObject>* RootAudioObject)
{
    FString AssetName = EventName + "_SoundBank";
    FString PackagePath = "/Game/SoundBanks/" + AssetName;
	FString MSPackagePath = "/Game/SoundBanks/MetaSounds/";
    UPackage* Package = CreatePackage(*PackagePath);

    // Create a new sound asset and set its properties
    UGisbSoundBankDataAsset* NewAsset = NewObject<UGisbSoundBankDataAsset>(Package, *AssetName, RF_Public | RF_Standalone);
    UGisbImportContainerBase* RootContainer = UGisbImportContainerBase::CreateFromJson(*RootAudioObject, NewAsset, FullPath);

    if (!RootContainer) return;
    
    NewAsset->EventName = EventName;
    NewAsset->RootContainer = RootContainer->ToRuntimeContainer(NewAsset);
    RootContainer->MarkPackageDirty();

    UMetaSoundSource* GeneratedMS = UGISB_MetasoundBuilder::CreateMetasoundFromGISB(RootContainer, EventName, MSPackagePath);
        
	NewAsset->GeneratedMS = GeneratedMS;

    NewAsset->Modify();  // Ensures Unreal tracks the change
		
    NewAsset->MarkPackageDirty();
    GeneratedMS->MarkPackageDirty();
    FAssetRegistryModule::AssetCreated(NewAsset);
		

    // Make sure the package is marked dirty for saving
    Package->MarkPackageDirty();

    // Save the asset
    const FString AssetFileName = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
    const FString PackageName = FPackageName::ObjectPathToPackageName(PackagePath);
    if (UEditorAssetLibrary::DoesAssetExist(PackageName))
    {
        UE_LOG(LogTemp, Error, TEXT("Asset already exists at: %s"), *AssetFileName);
    }
    else
    {
        FSavePackageArgs SaveArgs;
        SaveArgs.TopLevelFlags = RF_Standalone;
        SaveArgs.SaveFlags = EObjectFlags::RF_Public | EObjectFlags::RF_Standalone;
        SaveArgs.bWarnOfLongFilename = true;
        SaveArgs.bForceByteSwapping = false;
        bool bSaved = UPackage::SavePackage(Package, NewAsset, *AssetFileName, SaveArgs);

        if (bSaved)
        {
            UE_LOG(LogTemp, Log, TEXT("Asset successfully saved: %s"), *AssetFileName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to save the asset: %s"), *AssetFileName);
        }
    }
}