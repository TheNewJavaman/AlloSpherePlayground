// Copyright Gabriel Pizarro. All Rights Reserved.

using UnrealBuildTool;

public class AlloSphereShaders : ModuleRules
{
	public AlloSphereShaders(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new[]
		{
			"Core",
			"RenderCore",
			"RHI"
		});

		PrivateDependencyModuleNames.AddRange(new[]
		{
			"CoreUObject",
			"Engine",
			"Projects"
		});
	}
}