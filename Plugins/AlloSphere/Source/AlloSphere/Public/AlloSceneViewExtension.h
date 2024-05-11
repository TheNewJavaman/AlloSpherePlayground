// Copyright Gabriel Pizarro. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneViewExtension.h"

class UAlloSubsystem;

class FAlloSceneViewExtension : public FSceneViewExtensionBase
{
public:
	FAlloSceneViewExtension(const FAutoRegister& AutoRegister, TWeakObjectPtr<UAlloSubsystem> AlloSubsystem);

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SubscribeToPostProcessingPass(
		EPostProcessingPass Pass,
		FAfterPassCallbackDelegateArray& InOutPassCallbacks,
		bool bIsPassEnabled) override;

private:
	FScreenPassTexture PostProcessPassAfterFxaa_RenderThread(
		FRDGBuilder& GraphBuilder,
		const FSceneView& View,
		const FPostProcessMaterialInputs& Inputs);

	TWeakObjectPtr<UAlloSubsystem> AlloSubsystem;
};
