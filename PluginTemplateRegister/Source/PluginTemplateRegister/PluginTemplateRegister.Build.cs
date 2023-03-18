// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PluginTemplateRegister : ModuleRules
{
	public PluginTemplateRegister(ReadOnlyTargetRules Target) : base(Target)
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
				"Slate",
				"SlateCore",
				
				// Additional Modules
				"DeveloperSettings",
				"UnrealEd",
				"Projects",
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
