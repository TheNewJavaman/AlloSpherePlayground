// Copyright Gabriel Pizarro. All Rights Reserved.

#include "AlloSphereShadersModule.h"

#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FAlloSphereModule"

void FAlloSphereShadersModule::StartupModule()
{
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin(TEXT("AlloSphere"));
	check(Plugin);
	const FString PluginShaderDir = FPaths::Combine(Plugin->GetBaseDir(), TEXT("Shaders"));
	AddShaderSourceDirectoryMapping(TEXT("/Plugin/AlloSphere"), PluginShaderDir);
}

void FAlloSphereShadersModule::ShutdownModule()
{
	ResetAllShaderSourceDirectoryMappings();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAlloSphereShadersModule, AlloSphereShaders)
