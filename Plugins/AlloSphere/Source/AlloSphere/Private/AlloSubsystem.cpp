// Copyright Gabriel Pizarro. All Rights Reserved.

#include "AlloSubsystem.h"

#include "SceneViewExtension.h"

void UAlloSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UGameViewportClient* ViewportClient = GEngine->GameViewport)
	{
		// Disable main window rendering
		ViewportClient->bDisableWorldRendering = true;
	}

	AlloSceneViewExtension = FSceneViewExtensions::NewExtension<FAlloSceneViewExtension>();
}
