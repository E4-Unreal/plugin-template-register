// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "CoreMinimal.h"
#include "PluginTemplateRegisterSettings.generated.h"

/*
* Data for specifying a usable plugin template. 
*	-Plugin templates are a folder/file structure that are duplicated and renamed
*	 by the plugin creation wizard to easily create new plugins with a standard
*	 format.
* See PluginUtils.h for more information.
*/
USTRUCT()
struct FPluginTemplateData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = PluginTemplate, meta = (RelativePath))
	FDirectoryPath Path;

	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	FText Label;

	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	FText Description;

	/** If true, the created plugin will be enabled by default without needing to be added to the project file. */
	UPROPERTY(config, EditAnywhere, Category = Plugins)
	bool bIsEnabledByDefault = true;
	
	UPROPERTY(EditAnywhere, Category = PluginTemplate)
	TArray<FString> ModuleSuffixes;
};


UCLASS(config = PluginTemplateRegister, meta = (DisplayName = "Plugin Template Register"))
class PLUGINTEMPLATEREGISTER_API UPluginTemplateRegisterSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// Array of Plugin templates. Allows projects to specify reusable plugin templates for the plugin creation wizard.
	UPROPERTY(config, EditAnywhere, Category = Plugins)
	TArray<FPluginTemplateData> PluginTemplates;
};