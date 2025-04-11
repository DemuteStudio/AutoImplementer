// Fill out your copyright notice in the Description page of Project Settings.
#include "GisbImportContainerSimpleSound.h"

#include "Sound/SoundWave.h"
#include "Factories/SoundFactory.h"
#include "EditorAssetLibrary.h"
#include "GisbContainerSimpleSound.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Dom/JsonObject.h"

void UGisbImportContainerSimpleSound::ParseJson(const TSharedPtr<FJsonObject>& JsonObject, UObject* Outer, const FString& path)
{
    AudioFilePath = JsonObject->GetStringField("soundClip");
    loop = FCString::ToBool(*JsonObject->GetStringField("soundClip"));

    // Check if the audio file exists
    FString AbsoluteAudioFilePath = FPaths::Combine(FPaths::GetPath(path), TEXT("Audio Files"), AudioFilePath);

    if (FPaths::FileExists(AbsoluteAudioFilePath))
    {
        // Define the package path where the asset will be stored
        FString PackageName = TEXT("/Game/ImportedAudio/") + FPaths::GetBaseFilename(AbsoluteAudioFilePath);
        FString PackagePath = FPackageName::ObjectPathToPackageName(PackageName);

        // Check if the asset already exists
        if (UEditorAssetLibrary::DoesAssetExist(PackageName))
        {
            UE_LOG(LogTemp, Warning, TEXT("Asset already exists at path: %s"), *PackageName);
            return;
        }

        // Create a new package for the asset
        UPackage* Package = CreatePackage(*PackagePath);
        if (!Package)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create package at path: %s"), *PackagePath);
            return;
        }

        // Create a new SoundFactory
        USoundFactory* SoundFactory = NewObject<USoundFactory>(USoundFactory::StaticClass());
        if (!SoundFactory)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to create SoundFactory"));
            return;
        }

        // Import the sound file as a USoundWave asset
        bool IsCancelled = false;
        USoundWave* ImportedSoundWave = Cast<USoundWave>(SoundFactory->FactoryCreateFile(
            USoundWave::StaticClass(),
            Package,
            *FPaths::GetBaseFilename(AbsoluteAudioFilePath),
            RF_Public | RF_Standalone,
            AbsoluteAudioFilePath,
            nullptr,
            GWarn,
            IsCancelled
        ));

        if (ImportedSoundWave)
        {
            Package->MarkPackageDirty();
            
            FAssetRegistryModule::AssetCreated(ImportedSoundWave);
            
            SoundWave = ImportedSoundWave;

            UE_LOG(LogTemp, Log, TEXT("Successfully imported sound file: %s"), *AbsoluteAudioFilePath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to import sound file: %s"), *AbsoluteAudioFilePath);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Sound file not found at path: %s"), *AbsoluteAudioFilePath);
    }
}

UGisbContainerBase* UGisbImportContainerSimpleSound::ToRuntimeContainer(UObject* Outer)
{
    UGisbContainerSimpleSound* RuntimeContainer = NewObject<UGisbContainerSimpleSound>(Outer, UGisbContainerSimpleSound::StaticClass(), NAME_None, RF_Public | RF_Transactional | RF_Standalone);
    
    RuntimeContainer->Sound = SoundWave;
    RuntimeContainer->loop = loop;
    RuntimeContainer->MarkPackageDirty();
    return RuntimeContainer;
}
