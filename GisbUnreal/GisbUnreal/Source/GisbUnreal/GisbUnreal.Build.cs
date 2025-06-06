// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GisbUnreal : ModuleRules
{
	public GisbUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"Json", 
			"JsonUtilities",
			"DesktopPlatform",
			"Slate", 
			"SlateCore", 
			"EditorScriptingUtilities", 
			"AssetTools", 
			"UnrealEd",
			"AudioEditor",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
    		"MetasoundEngine", "MetasoundFrontend", "MetasoundGraphCore", "MetasoundEditor"
		});


		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
