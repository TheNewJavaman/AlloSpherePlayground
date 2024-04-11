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
	BEGIN_SHADER_PARAMETER_STRUCT(FNativeDispatchPassParameters,)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D, InteropTexture)
	END_SHADER_PARAMETER_STRUCT()

	FScreenPassTexture PostProcessPassAfterFxaa_RenderThread(
		FRDGBuilder& GraphBuilder,
		const FSceneView& View, const FPostProcessMaterialInputs& Inputs);

	TWeakObjectPtr<UAlloSubsystem> AlloSubsystem;
};
