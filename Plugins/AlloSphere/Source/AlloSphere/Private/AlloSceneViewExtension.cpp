// Copyright Gabriel Pizarro. All Rights Reserved.

#include "AlloSceneViewExtension.h"

#include "AlloCaptureComponent.h"
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
	FScreenPassTexture SceneColor = Inputs.GetInput(EPostProcessMaterialInput::SceneColor);

	const UAlloCaptureComponent* CaptureComponent = Cast<UAlloCaptureComponent>(View.ViewActor);
	if (!CaptureComponent)
	{
		return SceneColor;
	}

	// Interop copy pass: copy scene color to an intermediate texture that can be accessed externally by OpenGL
	FRDGTextureRef InteropTexture;
	{
		FRDGTextureDesc InteropDesc = SceneColor.Texture->Desc;
		InteropDesc.Flags |= ETextureCreateFlags::External;
		InteropTexture = GraphBuilder.CreateTexture(InteropDesc, TEXT("AlloSphere.InteropTexture"));
		AddCopyTexturePass(GraphBuilder, SceneColor.Texture, InteropTexture);
	}

	// Native dispatch pass: copy Vulkan texture to OpenGL AlloLib framebuffer
	{
		FNativeDispatchPassParameters* Parameters = GraphBuilder.AllocParameters<FNativeDispatchPassParameters>();
		Parameters->InteropTexture = InteropTexture;
		GraphBuilder.AddPass(
			RDG_EVENT_NAME("AlloSphere.NativeDispatchPass"),
			Parameters,
			ERDGPassFlags::Raster | ERDGPassFlags::NeverCull,
			[Parameters](FRHICommandListImmediate& RHICmdList)
			{
				// Todo: Native AlloLib stuff
			}
		);
	}

	return SceneColor;
}
