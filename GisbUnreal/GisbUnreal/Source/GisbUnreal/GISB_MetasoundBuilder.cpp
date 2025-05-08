// Fill out your copyright notice in the Description page of Project Settings.


#include "GISB_MetasoundBuilder.h"
#include "MetasoundSource.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "AssetRegistry/AssetRegistryModule.h"

UMetaSoundSource* UGISB_MetasoundBuilder::CreateMetasoundFromGISB(UGisbImportContainerBase* gisb, const FString& Name, const FString& path)
{
	UPackage* MSPackage = CreatePackage(*path);
	UMetaSoundSource* GeneratedMS = NewObject<UMetaSoundSource>(MSPackage, *Name, RF_Public | RF_Standalone);
	GeneratedMS->Modify();
	FAssetRegistryModule::AssetCreated(GeneratedMS);

	//Do stuff here

	MSPackage->MarkPackageDirty();
	const FString AssetMSFileName = FPackageName::LongPackageNameToFilename(path, FPackageName::GetAssetPackageExtension());
	UPackage::SavePackage(MSPackage, GeneratedMS, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *AssetMSFileName);
	return GeneratedMS;
}


