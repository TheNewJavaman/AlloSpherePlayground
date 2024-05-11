// Copyright Gabriel Pizarro. All Rights Reserved.

#include "AlloSceneViewExtension.h"

#include "AlloShaders.h"
#include "AlloSubsystem.h"
#include "ScreenPass.h"
#include "PostProcess/PostProcessMaterialInputs.h"

FAlloSceneViewExtension::FAlloSceneViewExtension(
	const FAutoRegister& AutoRegister,
	const TWeakObjectPtr<UAlloSubsystem> AlloSubsystem)
	: FSceneViewExtensionBase(AutoRegister),
	  AlloSubsystem(AlloSubsystem)
{
}

void FAlloSceneViewExtension::SetupViewFamily(FSceneViewFamily& InViewFamily)
{
}

void FAlloSceneViewExtension::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
}

void FAlloSceneViewExtension::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{
}

void FAlloSceneViewExtension::SubscribeToPostProcessingPass(
	const EPostProcessingPass Pass,
	FAfterPassCallbackDelegateArray& InOutPassCallbacks,
	bool bIsPassEnabled)
{
	if (Pass == EPostProcessingPass::FXAA)
	{
		InOutPassCallbacks.Add(
			FAfterPassCallbackDelegate::CreateRaw(
				this,
				&FAlloSceneViewExtension::PostProcessPassAfterFxaa_RenderThread));
	}
}

FScreenPassTexture FAlloSceneViewExtension::PostProcessPassAfterFxaa_RenderThread(
	FRDGBuilder& GraphBuilder,
	const FSceneView& View,
	const FPostProcessMaterialInputs& Inputs)
{
	const FScreenPassTexture& SceneColor = FScreenPassTexture::CopyFromSlice(
		GraphBuilder, Inputs.GetInput(EPostProcessMaterialInput::SceneColor));

	const FRDGTextureDesc OutColorDesc = FRDGTextureDesc::Create2D(
		SceneColor.ViewRect.Size(),
		PF_FloatRGBA,
		FClearValueBinding::None,
		ETextureCreateFlags::ShaderResource | ETextureCreateFlags::RenderTargetable);
	FRDGTextureRef OutColorTexture = GraphBuilder.CreateTexture(OutColorDesc, TEXT("AlloSphere.OutColorTexture"));

	const auto Parameters = GraphBuilder.AllocParameters<FAlloWarpBlendShaderPS::FParameters>();
	Parameters->InvTanHalfFov = 1.0 / FMath::Tan(AlloSubsystem->Info.FOV * PI / 180.0 / 2.0);
	Parameters->ColorTexture = SceneColor.Texture;
	Parameters->ColorSampler = TStaticSamplerState<>::GetRHI();
	Parameters->RenderTargets.Output[0].SetTexture(OutColorTexture);

	AddDrawScreenPass<FAlloWarpBlendShaderPS>(
		GraphBuilder,
		RDG_EVENT_NAME("AlloSphere.WarpBlendPass"),
		View,
		FScreenPassTextureViewport(OutColorTexture),
		FScreenPassTextureViewport(SceneColor),
		GetGlobalShaderMap(ERHIFeatureLevel::SM5)->GetShader<FAlloWarpBlendShaderPS>(),
		Parameters);

	return FScreenPassTexture(OutColorTexture);
}
