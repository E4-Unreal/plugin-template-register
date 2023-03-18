// Copyright Epic Games, Inc. All Rights Reserved.

#include "PluginTemplateRegisterModule.h"

#include "Features/EditorFeatures.h"
#include "PluginDescriptor.h"
#include "PluginTemplateRegisterSettings.h"

#define LOCTEXT_NAMESPACE "FPluginTemplateRegisterModule"

//////////////////////////////////////////////////////////////////////

struct FCustomPluginTemplateDescription : public FPluginTemplateDescription
{
	FCustomPluginTemplateDescription(FText InName, FText InDescription, FString InOnDiskPath, EPluginEnabledByDefault InEnabledByDefault, TArray<FString> InModuleSuffixes)
		: FPluginTemplateDescription(InName, InDescription, InOnDiskPath, /*bCanContainContent=*/ true, EHostType::Runtime)
	{
		SortPriority = 10;
		bCanBePlacedInEngine = false;
		PluginEnabledByDefault = InEnabledByDefault;

		ModuleSuffixes = InModuleSuffixes;
	}

	virtual bool ValidatePathForPlugin(const FString& ProposedAbsolutePluginPath, FText& OutErrorMessage) override
	{
		if (!IsRootedInPluginsRoot(ProposedAbsolutePluginPath))
		{
			OutErrorMessage = LOCTEXT("InvalidPathForPlugin", "Plugin must be inside the Plugins folder");
			return false;
		}

		OutErrorMessage = FText::GetEmpty();
		return true;
	}

	virtual void UpdatePathWhenTemplateSelected(FString& InOutPath) override
	{
		if (!IsRootedInPluginsRoot(InOutPath))
		{
			InOutPath = GetPluginTemplateRegisterRoot();
		}
	}

	virtual void UpdatePathWhenTemplateUnselected(FString& InOutPath) override
	{
		InOutPath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(*FPaths::ProjectPluginsDir());
		FPaths::MakePlatformFilename(InOutPath);
	}

	// Customize .uplugin
	virtual void CustomizeDescriptorBeforeCreation(FPluginDescriptor& Descriptor) override
	{
		Descriptor.VersionName = "0.1";
		Descriptor.EngineVersion = "5.1.0";
		Descriptor.Category = TEXT("New");
		Descriptor.EnabledByDefault = PluginEnabledByDefault;
		Descriptor.bIsExperimentalVersion = true;

		if (ModuleSuffixes.Num() > 0)
		{
			// To make ModuleSuffixes.Num() == Descriptor.Modules()
			for (int i = 1; i < ModuleSuffixes.Num(); i++)
			{
				Descriptor.Modules.Emplace(Descriptor.Modules[0]);
			}

			// Rename Modules with PluginName + Suffixes
			for (int i = 0; i < ModuleSuffixes.Num(); i++)
			{
				Descriptor.Modules[i].Name = FName(*(Descriptor.Modules[i].Name.ToString() + ModuleSuffixes[i]));
				
				if (ModuleSuffixes[i] == "Editor") // For Editor module
				{
					Descriptor.Modules[i].Type = EHostType::Editor;
				}
				else if (ModuleSuffixes[i] == "GameplayTags") // For Native Gameplay Tag
				{
					Descriptor.Modules[i].LoadingPhase = ELoadingPhase::PostSplashScreen;
					// Descriptor.Modules[i].LoadingPhase = ELoadingPhase::PreEarlyLoadingScreen;
				}
			}
		}
	}

	virtual void OnPluginCreated(TSharedPtr<IPlugin> NewPlugin) override
	{
		// TODO
	}

	FString GetPluginTemplateRegisterRoot() const
	{
		FString Result = IFileManager::Get().ConvertToAbsolutePathForExternalAppForWrite(*(FPaths::ProjectPluginsDir() / TEXT("PluginTemplateRegister/")));
		FPaths::MakePlatformFilename(Result);
		return Result;
	}

	bool IsRootedInPluginsRoot(const FString& InStr)
	{
		const FString ConvertedPath = FPaths::ConvertRelativePathToFull(FPaths::CreateStandardFilename(InStr / TEXT("test.uplugin")));
		const FString PluginsFolder = FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir());

		if (ConvertedPath.StartsWith(PluginsFolder))
		{
			return true;
		}

		return false;
	}

	EPluginEnabledByDefault PluginEnabledByDefault = EPluginEnabledByDefault::Disabled;
	TArray<FString> ModuleSuffixes;
};

//////////////////////////////////////////////////////////////////////

void FPluginTemplateRegisterModule::StartupModule()
{
	// Add templates to the new plugin wizard
	{

		CachePluginTemplates();

		IModularFeatures& ModularFeatures = IModularFeatures::Get();
		ModularFeatures.OnModularFeatureRegistered().AddRaw(this, &FPluginTemplateRegisterModule::OnModularFeatureRegistered);
		ModularFeatures.OnModularFeatureUnregistered().AddRaw(this, &FPluginTemplateRegisterModule::OnModularFeatureUnregistered);

		if (ModularFeatures.IsModularFeatureAvailable(EditorFeatures::PluginsEditor))
		{
			OnModularFeatureRegistered(EditorFeatures::PluginsEditor, &ModularFeatures.GetModularFeature<IPluginsEditorFeature>(EditorFeatures::PluginsEditor));
		}
	}
}

void FPluginTemplateRegisterModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FPluginTemplateRegisterModule::CachePluginTemplates()
{
	PluginTemplates.Reset();
	// Make CustomPluginTemplateDescription from DefaultPluginTemplateRegister.ini
	if (const UPluginTemplateRegisterSettings* PluginEditorSettings = GetDefault<UPluginTemplateRegisterSettings>())
	{
		for (const FPluginTemplateData& PluginTemplate : PluginEditorSettings->PluginTemplates)
		{
			PluginTemplates.Add(MakeShareable(new FCustomPluginTemplateDescription(
				PluginTemplate.Label,
				PluginTemplate.Description,
				FPaths::ProjectPluginsDir() + PluginTemplate.Path.Path,
				PluginTemplate.bIsEnabledByDefault ? EPluginEnabledByDefault::Enabled : EPluginEnabledByDefault::Disabled,
				PluginTemplate.ModuleSuffixes
			)));
		}
	}
}

void FPluginTemplateRegisterModule::ResetPluginTemplates()
{
	UnregisterFunctionTemplates();
	CachePluginTemplates();
	RegisterPluginTemplates();
}

void FPluginTemplateRegisterModule::RegisterPluginTemplates()
{
	if (IModularFeatures::Get().IsModularFeatureAvailable(EditorFeatures::PluginsEditor))
	{
		IPluginsEditorFeature& PluginEditor = IModularFeatures::Get().GetModularFeature<IPluginsEditorFeature>(EditorFeatures::PluginsEditor);
		for (const TSharedPtr<FCustomPluginTemplateDescription, ESPMode::ThreadSafe>& TemplateDescription : PluginTemplates)
		{
			PluginEditor.RegisterPluginTemplate(TemplateDescription.ToSharedRef());
		}
	}
}

void FPluginTemplateRegisterModule::UnregisterFunctionTemplates()
{
	if (IModularFeatures::Get().IsModularFeatureAvailable(EditorFeatures::PluginsEditor))
	{
		IPluginsEditorFeature& PluginEditor = IModularFeatures::Get().GetModularFeature<IPluginsEditorFeature>(EditorFeatures::PluginsEditor);
		for (const TSharedPtr<FCustomPluginTemplateDescription, ESPMode::ThreadSafe>& TemplateDescription : PluginTemplates)
		{
			PluginEditor.UnregisterPluginTemplate(TemplateDescription.ToSharedRef());
		}
	}

}

void FPluginTemplateRegisterModule::OnModularFeatureRegistered(const FName& Type, class IModularFeature* ModularFeature)
{
	if (Type == EditorFeatures::PluginsEditor)
	{
		ResetPluginTemplates();
	}
}

void FPluginTemplateRegisterModule::OnModularFeatureUnregistered(const FName& Type, class IModularFeature* ModularFeature)
{
	if (Type == EditorFeatures::PluginsEditor)
	{
		UnregisterFunctionTemplates();
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPluginTemplateRegisterModule, PluginTemplateRegister)