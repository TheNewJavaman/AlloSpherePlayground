// Copyright Gabriel Pizarro. All Rights Reserved.

using UnrealBuildTool;

public class AlloSphere : ModuleRules
{
	public AlloSphere(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"AlloSphereShaders",
			"CoreUObject",
			"Engine",
			"RHI",
			"RHICore",
			"Renderer",
			"RenderCore"
		});
		
		PublicIncludePaths.AddRange(new[]
		{
			"/usr/include"
		});
	}
}