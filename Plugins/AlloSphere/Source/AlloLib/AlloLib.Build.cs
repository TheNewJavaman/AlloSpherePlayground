// Copyright Gabriel Pizarro. All Rights Reserved.

using System.IO;
using UnrealBuildTool;
 
public class AlloLib : ModuleRules
{
	public AlloLib(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;
 
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));
		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "al.lib"));
	}
}
