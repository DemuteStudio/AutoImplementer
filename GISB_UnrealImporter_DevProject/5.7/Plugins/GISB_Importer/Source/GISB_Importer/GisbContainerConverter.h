// Copyright Demute SRL. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GisbContainerBase.h"
#include "GisbContainerSimpleSound.h"
#include "GisbContainerRandom.h"
#include "GisbContainerBlend.h"
#include "GisbContainerSwitch.h"
#include "GisbContainerTrigger.h"
#include "GisbImportContainerBase.h"
#include "GisbImportContainerSimpleSound.h"
#include "GisbImportContainerRandom.h"
#include "GisbImportContainerBlend.h"
#include "GisbImportContainerSwitch.h"
#include "GisbImportContainerTrigger.h"
#include "GisbContainerConverter.generated.h"

/**
 * Static utility class for converting between runtime and import containers.
 * Primary use case: Testing MetaSound builder functions that require import containers.
 *
 * The converter transforms runtime containers (UGisbContainerBase hierarchy) back into
 * import containers (UGisbImportContainerBase hierarchy), enabling testing of MetaSound
 * builder functions like BuildSimpleSoundNode() which expect import container parameters.
 */
UCLASS()
class GISB_IMPORTER_API UGisbContainerConverter : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Converts a runtime container to an import container (reverse of ToRuntimeContainer).
	 * Handles all container types polymorphically.
	 *
	 * @param RuntimeContainer - The runtime container to convert
	 * @param Outer - The outer object for the new import container (defaults to GetTransientPackage())
	 * @return The corresponding import container, or nullptr if conversion fails
	 *
	 * Note: Maintains local reference to prevent premature GC. Uses RF_Public | RF_Standalone flags.
	 */
	static UGisbImportContainerBase* RuntimeToImportContainer(
		UGisbContainerBase* RuntimeContainer,
		UObject* Outer = nullptr
	);

	/**
	 * Converts UGisbContainerSimpleSound to UGisbImportContainerSimpleSound.
	 * Note: AudioFilePath will be empty (data loss, but acceptable for testing).
	 */
	static UGisbImportContainerSimpleSound* SimpleSoundToImport(
		UGisbContainerSimpleSound* RuntimeContainer,
		UObject* Outer
	);

	/**
	 * Converts UGisbContainerRandom to UGisbImportContainerRandom.
	 * Recursively converts all child containers.
	 */
	static UGisbImportContainerRandom* RandomSoundToImport(
		UGisbContainerRandom* RuntimeContainer,
		UObject* Outer
	);

	/**
	 * Converts UGisbContainerBlend to UGisbImportContainerBlend.
	 * Recursively converts all child containers.
	 */
	static UGisbImportContainerBlend* BlendSoundToImport(
		UGisbContainerBlend* RuntimeContainer,
		UObject* Outer
	);

	/**
	 * Converts UGisbContainerSwitch to UGisbImportContainerSwitch.
	 * Recursively converts all child containers in the switch map.
	 */
	static UGisbImportContainerSwitch* SwitchSoundToImport(
		UGisbContainerSwitch* RuntimeContainer,
		UObject* Outer
	);

	/**
	 * Converts UGisbContainerTrigger to UGisbImportContainerTrigger.
	 * Recursively converts the triggered child container.
	 */
	static UGisbImportContainerTrigger* TriggerSoundToImport(
		UGisbContainerTrigger* RuntimeContainer,
		UObject* Outer
	);

private:
	/**
	 * Copies base properties from runtime container to import container.
	 * Mirrors AssignBaseVariables() but in reverse direction.
	 */
	static void CopyBaseProperties(
		UGisbContainerBase* RuntimeContainer,
		UGisbImportContainerBase* ImportContainer
	);

	/**
	 * Generates the Type field string based on runtime container class.
	 * Maps runtime class types to JSON type strings.
	 */
	static FString GetTypeStringForRuntimeContainer(UGisbContainerBase* RuntimeContainer);

	/**
	 * Ensures Outer is valid, defaulting to transient package if nullptr.
	 */
	static UObject* GetValidOuter(UObject* Outer);
};
