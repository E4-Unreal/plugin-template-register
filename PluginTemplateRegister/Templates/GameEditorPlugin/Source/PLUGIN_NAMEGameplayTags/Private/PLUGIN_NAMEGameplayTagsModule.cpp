// Copyright Epic Games, Inc. All Rights Reserved.

#include "PLUGIN_NAMEGameplayTagsModule.h"
#include "PLUGIN_NAMEGameplayTags.h"

#define LOCTEXT_NAMESPACE "FPLUGIN_NAMEGameplayTagsModule"

void FPLUGIN_NAMEGameplayTagsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory;
	// the exact timing is specified in the .uplugin file per-module

	//Load Native Tags
	FPLUGIN_NAMEGameplayTags::InitializeNativeTags();
}

void FPLUGIN_NAMEGameplayTagsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.
	// For modules that support dynamic reloading, we call this function before unloading the module.
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FPLUGIN_NAMEGameplayTagsModule, PLUGIN_NAMEGameplayTags)
