// Copyright Gabriel Pizarro. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShaderParameterStruct.h"

class FAlloWarpBlendShaderPS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FAlloWarpBlendShaderPS);
	SHADER_USE_PARAMETER_STRUCT(FAlloWarpBlendShaderPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters,)
		SHADER_PARAMETER(float, InvTanHalfFov)
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D, ColorTexture)
		SHADER_PARAMETER_SAMPLER(SamplerState, ColorSampler)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()
};

IMPLEMENT_GLOBAL_SHADER(FAlloWarpBlendShaderPS, "/Plugin/AlloSphere/Private/AlloWarpBlendPS.usf", "MainPS", SF_Pixel);
