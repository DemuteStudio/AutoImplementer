// Copyright Demute SRL. All Rights Reserved.

#include "GisbContainerConverter.h"

UGisbImportContainerBase* UGisbContainerConverter::RuntimeToImportContainer(
	UGisbContainerBase* RuntimeContainer,
	UObject* Outer)
{
	if (!RuntimeContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("RuntimeToImportContainer: RuntimeContainer is null"));
		return nullptr;
	}

	Outer = GetValidOuter(Outer);

	// Polymorphic dispatch based on runtime type
	if (UGisbContainerSimpleSound* SimpleSound = Cast<UGisbContainerSimpleSound>(RuntimeContainer))
	{
		return SimpleSoundToImport(SimpleSound, Outer);
	}
	else if (UGisbContainerRandom* RandomSound = Cast<UGisbContainerRandom>(RuntimeContainer))
	{
		return RandomSoundToImport(RandomSound, Outer);
	}
	else if (UGisbContainerBlend* BlendSound = Cast<UGisbContainerBlend>(RuntimeContainer))
	{
		return BlendSoundToImport(BlendSound, Outer);
	}
	else if (UGisbContainerSwitch* SwitchSound = Cast<UGisbContainerSwitch>(RuntimeContainer))
	{
		return SwitchSoundToImport(SwitchSound, Outer);
	}
	else if (UGisbContainerTrigger* TriggerSound = Cast<UGisbContainerTrigger>(RuntimeContainer))
	{
		return TriggerSoundToImport(TriggerSound, Outer);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RuntimeToImportContainer: Unknown container type: %s"),
			   *RuntimeContainer->GetClass()->GetName());
		return nullptr;
	}
}

UGisbImportContainerSimpleSound* UGisbContainerConverter::SimpleSoundToImport(
	UGisbContainerSimpleSound* RuntimeContainer,
	UObject* Outer)
{
	if (!RuntimeContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("SimpleSoundToImport: RuntimeContainer is null"));
		return nullptr;
	}

	// Create new import container with RF_Public | RF_Transient (accessible but GC'd when unreferenced)
	UGisbImportContainerSimpleSound* ImportContainer = NewObject<UGisbImportContainerSimpleSound>(
		Outer,
		UGisbImportContainerSimpleSound::StaticClass(),
		NAME_None,
		RF_Public | RF_Transient
	);

	// Copy base properties (attenuation, volume, pitch, lowpass, probability)
	CopyBaseProperties(RuntimeContainer, ImportContainer);

	// Copy specific properties
	ImportContainer->loop = RuntimeContainer->loop;

	// Handle Sound → SoundWave conversion
	if (RuntimeContainer->Sound)
	{
		// Attempt to cast USoundBase to USoundWave
		ImportContainer->SoundWave = Cast<USoundWave>(RuntimeContainer->Sound);

		if (!ImportContainer->SoundWave)
		{
			UE_LOG(LogTemp, Warning,
				   TEXT("SimpleSoundToImport: Sound is not a SoundWave, type is: %s"),
				   *RuntimeContainer->Sound->GetClass()->GetName());
		}
	}

	// AudioFilePath remains empty (acceptable data loss for testing)
	ImportContainer->AudioFilePath = FString();

	// Set Type field
	ImportContainer->Type = GetTypeStringForRuntimeContainer(RuntimeContainer);

	// Compute cached properties now that all data is populated
	ImportContainer->ComputeCachedProperties();

	return ImportContainer;
}

UGisbImportContainerRandom* UGisbContainerConverter::RandomSoundToImport(
	UGisbContainerRandom* RuntimeContainer,
	UObject* Outer)
{
	if (!RuntimeContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("RandomSoundToImport: RuntimeContainer is null"));
		return nullptr;
	}

	UGisbImportContainerRandom* ImportContainer = NewObject<UGisbImportContainerRandom>(
		Outer,
		UGisbImportContainerRandom::StaticClass(),
		NAME_None,
		RF_Public | RF_Transient
	);

	CopyBaseProperties(RuntimeContainer, ImportContainer);

	// Copy specific property
	ImportContainer->AvoidLastPlayed = RuntimeContainer->AvoidLastPlayed;

	// Recursively convert children (CRITICAL for composite pattern)
	for (UGisbContainerBase* Child : RuntimeContainer->ChildContainers)
	{
		if (Child)
		{
			// Recursive call - handles any child container type
			UGisbImportContainerBase* ImportChild = RuntimeToImportContainer(Child, ImportContainer);
			if (ImportChild)
			{
				ImportContainer->SoundArray.Add(ImportChild);
			}
		}
	}

	ImportContainer->Type = GetTypeStringForRuntimeContainer(RuntimeContainer);

	// Compute cached properties - aggregates from converted children
	ImportContainer->ComputeCachedProperties();

	return ImportContainer;
}

UGisbImportContainerBlend* UGisbContainerConverter::BlendSoundToImport(
	UGisbContainerBlend* RuntimeContainer,
	UObject* Outer)
{
	if (!RuntimeContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("BlendSoundToImport: RuntimeContainer is null"));
		return nullptr;
	}

	UGisbImportContainerBlend* ImportContainer = NewObject<UGisbImportContainerBlend>(
		Outer,
		UGisbImportContainerBlend::StaticClass(),
		NAME_None,
		RF_Public | RF_Transient
	);

	CopyBaseProperties(RuntimeContainer, ImportContainer);

	// Recursively convert children
	for (UGisbContainerBase* Child : RuntimeContainer->ChildContainers)
	{
		if (Child)
		{
			UGisbImportContainerBase* ImportChild = RuntimeToImportContainer(Child, ImportContainer);
			if (ImportChild)
			{
				ImportContainer->SoundArray.Add(ImportChild);
			}
		}
	}

	ImportContainer->Type = GetTypeStringForRuntimeContainer(RuntimeContainer);

	// Compute cached properties - aggregates from converted children
	ImportContainer->ComputeCachedProperties();

	return ImportContainer;
}

UGisbImportContainerSwitch* UGisbContainerConverter::SwitchSoundToImport(
	UGisbContainerSwitch* RuntimeContainer,
	UObject* Outer)
{
	if (!RuntimeContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("SwitchSoundToImport: RuntimeContainer is null"));
		return nullptr;
	}

	UGisbImportContainerSwitch* ImportContainer = NewObject<UGisbImportContainerSwitch>(
		Outer,
		UGisbImportContainerSwitch::StaticClass(),
		NAME_None,
		RF_Public | RF_Transient
	);

	CopyBaseProperties(RuntimeContainer, ImportContainer);

	// Copy specific properties with field name mapping
	ImportContainer->ParameterID = RuntimeContainer->ParameterName;
	ImportContainer->DefaultParameterValue = RuntimeContainer->ParameterValue;

	// Recursively convert switch map children
	for (const TPair<FString, TObjectPtr<UGisbContainerBase>>& Pair : RuntimeContainer->SwitchMap)
	{
		if (Pair.Value)
		{
			UGisbImportContainerBase* ImportChild = RuntimeToImportContainer(Pair.Value, ImportContainer);
			if (ImportChild)
			{
				ImportContainer->SoundDictionary.Add(Pair.Key, ImportChild);
			}
		}
	}

	ImportContainer->Type = GetTypeStringForRuntimeContainer(RuntimeContainer);

	// Compute cached properties - aggregates from converted children
	ImportContainer->ComputeCachedProperties();

	return ImportContainer;
}

UGisbImportContainerTrigger* UGisbContainerConverter::TriggerSoundToImport(
	UGisbContainerTrigger* RuntimeContainer,
	UObject* Outer)
{
	if (!RuntimeContainer)
	{
		UE_LOG(LogTemp, Warning, TEXT("TriggerSoundToImport: RuntimeContainer is null"));
		return nullptr;
	}

	UGisbImportContainerTrigger* ImportContainer = NewObject<UGisbImportContainerTrigger>(
		Outer,
		UGisbImportContainerTrigger::StaticClass(),
		NAME_None,
		RF_Public | RF_Transient
	);

	// Copy base properties (attenuation, volume, pitch, lowpass, probability)
	CopyBaseProperties(RuntimeContainer, ImportContainer);

	// Copy trigger-specific properties
	ImportContainer->TransitionMode = RuntimeContainer->TransitionMode;
	ImportContainer->TriggerRate = RuntimeContainer->TriggerRate;
	ImportContainer->TriggerAmount = RuntimeContainer->TriggerAmount;
	ImportContainer->CrossfadeDuration = RuntimeContainer->CrossfadeDuration;

	// Recursively convert the triggered child sound
	if (RuntimeContainer->TriggeredSound)
	{
		UGisbImportContainerBase* ImportChild = RuntimeToImportContainer(
			RuntimeContainer->TriggeredSound,
			ImportContainer
		);
		if (ImportChild)
		{
			ImportContainer->TriggeredSoundImport = ImportChild;
		}
	}

	// Set Type field
	ImportContainer->Type = GetTypeStringForRuntimeContainer(RuntimeContainer);

	// Compute cached properties - inherits from converted child
	ImportContainer->ComputeCachedProperties();

	return ImportContainer;
}

void UGisbContainerConverter::CopyBaseProperties(
	UGisbContainerBase* RuntimeContainer,
	UGisbImportContainerBase* ImportContainer)
{
	if (!RuntimeContainer || !ImportContainer)
	{
		return;
	}

	// Direct struct copy (structs are identical between runtime and import)
	ImportContainer->Attenuation = RuntimeContainer->Attenuation;
	ImportContainer->VolumeDB = RuntimeContainer->VolumeDB;
	ImportContainer->Pitch = RuntimeContainer->Pitch;
	ImportContainer->Lowpass = RuntimeContainer->Lowpass;
	ImportContainer->PlaybackProbabilityPercent = RuntimeContainer->PlaybackProbabilityPercent;
}

FString UGisbContainerConverter::GetTypeStringForRuntimeContainer(UGisbContainerBase* RuntimeContainer)
{
	if (!RuntimeContainer)
	{
		return FString();
	}

	// Map runtime class names to JSON $type strings
	// These strings must match those in GisbImportContainerBase.cpp:16-29
	if (RuntimeContainer->IsA<UGisbContainerSimpleSound>())
	{
		return TEXT("GISB.Runtime.GISB_SingleSound, Demute.GISB");
	}
	else if (RuntimeContainer->IsA<UGisbContainerRandom>())
	{
		return TEXT("GISB.Runtime.GISB_RandomSound, Demute.GISB");
	}
	else if (RuntimeContainer->IsA<UGisbContainerSwitch>())
	{
		return TEXT("GISB.Runtime.GISB_SwitchSound, Demute.GISB");
	}
	else if (RuntimeContainer->IsA<UGisbContainerBlend>())
	{
		return TEXT("GISB.Runtime.GISB_BlendSound, Demute.GISB");
	}
	else if (RuntimeContainer->IsA<UGisbContainerTrigger>())
	{
		return TEXT("GISB.Runtime.GISB_TriggerSound, Demute.GISB");
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GetTypeStringForRuntimeContainer: Unknown type: %s"),
			   *RuntimeContainer->GetClass()->GetName());
		return FString();
	}
}

UObject* UGisbContainerConverter::GetValidOuter(UObject* Outer)
{
	// Use transient package if no outer specified
	// Transient package is appropriate for testing objects that won't be saved
	return Outer ? Outer : GetTransientPackage();
}
