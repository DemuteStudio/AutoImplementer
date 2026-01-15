// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "GISB_ImporterStyle.h"

class FGISB_ImporterCommands : public TCommands<FGISB_ImporterCommands>
{
public:

	FGISB_ImporterCommands()
		: TCommands<FGISB_ImporterCommands>(TEXT("GISB_Importer"), NSLOCTEXT("Contexts", "GISB_Importer", "GISB_Importer Plugin"), NAME_None, FGISB_ImporterStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
