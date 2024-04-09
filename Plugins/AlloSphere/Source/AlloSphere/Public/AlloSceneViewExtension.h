// Copyright Gabriel Pizarro. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

class UAlloSubsystem;

class FAlloSceneViewExtension : public FSceneViewExtensionBase
{
public:
	FAlloSceneViewExtension(const FAutoRegister& AutoRegister);
	
	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;

private:
	TWeakPtr<UAlloSubsystem> AlloSubsystem;
};
