// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PLUGIN_NAMEGame : ModuleRules
{
	public PLUGIN_NAMEGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				// Default Modules
				"Core",
				
				// Additional Modules
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// Default Modules
				"CoreUObject",
				"Engine",

				// Additional Modules
				"GameplayTags",
				"PLUGIN_NAMEGameplayTags",
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// Additional Modules
			}
			);
	}
}
