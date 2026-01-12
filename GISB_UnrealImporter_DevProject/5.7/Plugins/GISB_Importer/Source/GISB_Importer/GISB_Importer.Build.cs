// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GISB_Importer : ModuleRules
{
	public GISB_Importer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				ModuleDirectory,
				// ... add public include paths required here ...
			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
				ModuleDirectory,
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Json",
				"JsonUtilities",
				"DesktopPlatform",
				"EditorScriptingUtilities",
				"AssetTools",
				"UnrealEd",
				"AudioEditor",
				"AudioMixer",
				"MetasoundFrontend",
				"AudioExperimentalRuntime",
				"MetasoundExperimentalRuntime",
				"MetasoundStandardNodes",
				"SignalProcessing",
				"MetasoundGenerator",
				"MetasoundGraphCore",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"EditorFramework",
				"UnrealEd",
				"ToolMenus",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"MetasoundEngine",
				"MetasoundFrontend",
				"MetasoundGraphCore",
				"MetasoundEditor", 
				"MetasoundExperimentalRuntime",
				"MetasoundGenerator",
				"MetasoundStandardNodes",
				"SignalProcessing",
				"AudioExperimentalRuntime"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
