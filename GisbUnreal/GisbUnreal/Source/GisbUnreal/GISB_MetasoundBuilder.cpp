// Fill out your copyright notice in the Description page of Project Settings.


#include "GISB_MetasoundBuilder.h"
#include "MetasoundSource.h"
#include "MetasoundFrontend.h"
#include "MetasoundFrontendDocument.h"
#include "MetasoundBuilderSubsystem.h"
#include "MetasoundBuilderBase.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "MetasoundEditorSubsystem.h"

UMetaSoundSource* UGISB_MetasoundBuilder::CreateMetasoundFromGISB(UGisbImportContainerBase* gisb, const FString& Name, const FString& path)
{
	bool canLoop = false;
	bool shouldStereo = false;
	DetectLoopAndMono(gisb, canLoop, shouldStereo);
	EMetaSoundOutputAudioFormat format = shouldStereo ? EMetaSoundOutputAudioFormat::Stereo : EMetaSoundOutputAudioFormat::Mono;
	
	EMetaSoundBuilderResult result;
	UMetaSoundBuilderSubsystem* BuilderSubsystem = GEngine->GetEngineSubsystem<UMetaSoundBuilderSubsystem>();
	FMetaSoundBuilderNodeOutputHandle OnPlayNode;
	FMetaSoundBuilderNodeInputHandle OnFinishedNode;
	TArray<FMetaSoundBuilderNodeInputHandle> outAudioHandles;
	UMetaSoundSourceBuilder* builder = BuilderSubsystem->CreateSourceBuilder(FName(Name), OnPlayNode, OnFinishedNode, outAudioHandles, result,format, !canLoop);
	builder->SetAuthor("GISB");
	
	SetupNodes();
	ConnectContainerToGraph(builder, gisb, &OnPlayNode, &OnFinishedNode, &outAudioHandles);

	FMetaSoundBuilderOptions options = FMetaSoundBuilderOptions();
	UMetaSoundEditorSubsystem* MySubsystem = GEditor->GetEditorSubsystem<UMetaSoundEditorSubsystem>();
	TScriptInterface<IMetaSoundDocumentInterface> ms = MySubsystem->BuildToAsset(builder, "GISB", Name, path, result);
	if (result == EMetaSoundBuilderResult::Failed)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to build UMetaSoundSource."))
	}
	UObject* Object = ms.GetObject();
	UMetaSoundSource* GeneratedMS = Cast<UMetaSoundSource>(Object);
	return GeneratedMS;
}

void UGISB_MetasoundBuilder::SetupNodes()
{
	
}

void UGISB_MetasoundBuilder::DetectLoopAndMono(UGisbImportContainerBase* gisb, bool& canLoop, bool& shouldStereo)
{
	if (canLoop && shouldStereo) return;

	UGisbImportContainerSimpleSound* simplesound = Cast<UGisbImportContainerSimpleSound>(gisb);
	if (simplesound != nullptr)
	{
		if(simplesound->loop) canLoop = true;
		if(simplesound->SoundWave->NumChannels > 1) shouldStereo = true;
	}
	
	UGisbImportContainerBlend* blend = Cast<UGisbImportContainerBlend>(gisb);
	if (blend != nullptr)
	{
		for (UGisbImportContainerBase* child : blend->SoundArray)
		{
			DetectLoopAndMono(child, canLoop, shouldStereo);
		}
	}

	UGisbImportContainerRandom* random = Cast<UGisbImportContainerRandom>(gisb);
	if (random != nullptr)
	{
		for (UGisbImportContainerBase* child : random->SoundArray)
		{
			DetectLoopAndMono(child, canLoop, shouldStereo);
		}
	}

	UGisbImportContainerSwitch* swtch = Cast<UGisbImportContainerSwitch>(gisb);
	if (swtch != nullptr)
	{
		for (auto& child : swtch->SoundDictionary)
		{
			DetectLoopAndMono(child.Value, canLoop, shouldStereo);
		}
	}
}

void UGISB_MetasoundBuilder::ConnectContainerToGraph(UMetaSoundSourceBuilder* builder, UGisbImportContainerBase* gisb, FMetaSoundBuilderNodeOutputHandle* executionHandle, FMetaSoundBuilderNodeInputHandle* finishHandle, TArray<FMetaSoundBuilderNodeInputHandle>* audioHandles)
{
	UGisbImportContainerSimpleSound* simplesound = Cast<UGisbImportContainerSimpleSound>(gisb);
	if (simplesound != nullptr)
	{
		//EMetaSoundBuilderResult result;
		//builder->AddNodeByClassName(WavePlayerMonoNode, result); // Uncomment when WavePlayerMonoNode is set up
		// Create simplesound node at the beginning and at the end
	}

	UGisbImportContainerBlend* blend = Cast<UGisbImportContainerBlend>(gisb);
	if (blend != nullptr)
	{
		// Create blend node at the beginning and at the end
		// Replicate for each child
	}

	UGisbImportContainerRandom* random = Cast<UGisbImportContainerRandom>(gisb);
	if (random != nullptr)
	{
		// Create random node at the beginning and at the end
		// Replicate for each child
	}

	UGisbImportContainerSwitch* swtch = Cast<UGisbImportContainerSwitch>(gisb);
	if (swtch != nullptr)
	{
		// Create switch node at the beginning and at the end
		// Replicate for each child
	}
}