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
	if (Pass == EPostProcessingPass::FXAA && AlloSubsystem->Calibrations.IsSet())
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
	const bool IsFirst = (*AlloSubsystem->Calibrations)[0].PlayerIndex == View.PlayerIndex;
	const FAlloCalibration& Calibration = (*AlloSubsystem->Calibrations)[IsFirst ? 0 : 1];

	const FScreenPassTexture& SceneColor = FScreenPassTexture::CopyFromSlice(
		GraphBuilder,
		Inputs.GetInput(EPostProcessMaterialInput::SceneColor));

	FRDGTextureRef OutColorTexture;
	{
		const FRDGTextureDesc Desc = FRDGTextureDesc::Create2D(
			SceneColor.ViewRect.Size(),
			PF_FloatRGBA,
			FClearValueBinding::None,
			ETextureCreateFlags::ShaderResource | ETextureCreateFlags::RenderTargetable);
		OutColorTexture = GraphBuilder.CreateTexture(Desc, TEXT("AlloSphere.OutColorTexture"));
	}

	FRDGTextureRef WarpBlendTexture;
	{
		if (!Calibration.WarpAndBlendTexture) return SceneColor;
		FTextureResource* TextureResource = Calibration.WarpAndBlendTexture->GetResource();
		if (!TextureResource) return SceneColor;
		FRHITexture* RHITexture = TextureResource->GetTexture2DRHI();
		if (!RHITexture) return SceneColor;
		
		WarpBlendTexture = RegisterExternalTexture(
			GraphBuilder,
			RHITexture,
			TEXT("AlloSphere.WarpAndBlendTexture"));
	}

	const auto Parameters = GraphBuilder.AllocParameters<FAlloWarpBlendShaderPS::FParameters>();
	Parameters->InvTanHalfFov = 1.0 / FMath::Tan(Calibration.FovInRad);
	Parameters->RotationMatrix = Calibration.RotationMatrix;
	Parameters->ColorTexture = SceneColor.Texture;
	Parameters->WarpBlendTexture = WarpBlendTexture;
	Parameters->CommonSampler = TStaticSamplerState<>::GetRHI();
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
