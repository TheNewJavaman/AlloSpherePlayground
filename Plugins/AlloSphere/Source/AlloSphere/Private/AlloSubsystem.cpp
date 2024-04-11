// Copyright Gabriel Pizarro. All Rights Reserved.

#include "AlloSubsystem.h"

#include "AlloCaptureComponent.h"
#include "SceneViewExtension.h"

void UAlloSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (UGameViewportClient* ViewportClient = GEngine->GameViewport)
	{
		// Disable main window rendering
		ViewportClient->bDisableWorldRendering = true;
	}

	SceneViewExtension = FSceneViewExtensions::NewExtension<FAlloSceneViewExtension>(MakeWeakObjectPtr(this));
}

void UAlloSubsystem::Tick(float DeltaTime)
{
	// Todo: Despawn capture components
	// Todo: Spawn capture components; set id, size, pose

	for (const TWeakObjectPtr<UAlloCaptureComponent>& CaptureComponent : CaptureComponents)
	{
		CaptureComponent->CaptureSceneDeferred();
	}
}
