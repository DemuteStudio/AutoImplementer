// Fill out your copyright notice in the Description page of Project Settings.

#include "GisbSoundBankDataAsset.h"

FPrimaryAssetId UGisbSoundBankDataAsset::GetPrimaryAssetId() const 
{
	return FPrimaryAssetId(AssetType, GetFName());
}