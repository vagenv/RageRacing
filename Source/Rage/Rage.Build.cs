// Copyright 2015 Vagen Ayrapetyan


using UnrealBuildTool;

public class Rage : ModuleRules
{
	public Rage(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
