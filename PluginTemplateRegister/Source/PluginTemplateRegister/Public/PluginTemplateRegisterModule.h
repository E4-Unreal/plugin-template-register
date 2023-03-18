// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Features/IPluginsEditorFeature.h"

struct FCustomPluginTemplateDescription;

class FPluginTemplateRegisterModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	void CachePluginTemplates();
	void ResetPluginTemplates();
	void RegisterPluginTemplates();
	void UnregisterFunctionTemplates();
	void OnModularFeatureRegistered(const FName& Type, IModularFeature* ModularFeature);
	void OnModularFeatureUnregistered(const FName& Type, IModularFeature* ModularFeature);

private:
	
	// Array of Plugin templates populated from GameFeatureDeveloperSettings. Allows projects to
	//	specify reusable plugin templates for the plugin creation wizard.
	TArray<TSharedPtr<FCustomPluginTemplateDescription>> PluginTemplates;
};
