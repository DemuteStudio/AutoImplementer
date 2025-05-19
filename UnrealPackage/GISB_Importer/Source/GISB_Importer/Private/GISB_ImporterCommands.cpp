// Copyright Epic Games, Inc. All Rights Reserved.

#include "GISB_ImporterCommands.h"

#define LOCTEXT_NAMESPACE "FGISB_ImporterModule"

void FGISB_ImporterCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "GISB_Importer", "Execute GISB_Importer action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
