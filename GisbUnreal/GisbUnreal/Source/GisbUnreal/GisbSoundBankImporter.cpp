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
            FString FullPath = FPaths::Combine(BaseDir, FilePath);
            FullPath = FPaths::ConvertRelativePathToFull(FullPath);
            
            UE_LOG(LogTemp, Error, TEXT("File path: %s"), *FilePath);
            // Parse JSON
            TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonContent);
            TSharedPtr<FJsonObject> JsonObject;

            if (FJsonSerializer::Deserialize(Reader, JsonObject))
            {
                FString EventName;
                if (!JsonObject->TryGetStringField("name", EventName))
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to get event name from JSON."));
                    return;
                }

                const TSharedPtr<FJsonObject>* RootAudioObject;
                if (JsonObject->TryGetObjectField("rootAudioObject", RootAudioObject))
                {
                    FString AssetName = EventName + "_SoundBank";
                    FString PackagePath = "/Game/SoundBanks/" + AssetName;
                    UPackage* Package = CreatePackage(*PackagePath);

                    // Create a new sound asset and set its properties
                    UGisbSoundBankDataAsset* NewAsset = NewObject<UGisbSoundBankDataAsset>(Package, *AssetName, RF_Public | RF_Standalone);
                    UGisbImportContainerBase* RootContainer = UGisbImportContainerBase::CreateFromJson(*RootAudioObject, NewAsset, FullPath);
                    
                    if (RootContainer)
                    {
                        NewAsset->EventName = EventName;
                        NewAsset->RootContainer = RootContainer->ToRuntimeContainer(NewAsset);
                        RootContainer->MarkPackageDirty();
                        
                        NewAsset->Modify();  // Ensures Unreal tracks the change
                        NewAsset->MarkPackageDirty();
                        FAssetRegistryModule::AssetCreated(NewAsset);

                        // Make sure the package is marked dirty for saving
                        Package->MarkPackageDirty();

                        // Save the asset
                        const FString AssetFileName = FPackageName::LongPackageNameToFilename(PackagePath, FPackageName::GetAssetPackageExtension());
                        bool bSaved = UPackage::SavePackage(Package, NewAsset, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *AssetFileName);

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
            }
        }
    }
}
