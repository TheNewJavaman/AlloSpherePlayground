// Copyright Gabriel Pizarro. All Rights Reserved.

#include "..\Public\AlloSphereModule.h"

#define LOCTEXT_NAMESPACE "FAlloSphereModule"

void FAlloSphereModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FAlloSphereModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAlloSphereModule, AlloSphere)