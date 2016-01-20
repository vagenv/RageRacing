// Copyright 2015-2016 Vagen Ayrapetyan


using UnrealBuildTool;

public class Rage : ModuleRules
{
	public Rage(TargetInfo Target)
	{
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;



		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HTTP", "OnlineSubsystem" , "OnlineSubsystemUtils" ,"UMG"});

        //Uncomment if you are using online features
        PrivateDependencyModuleNames.Add("OnlineSubsystem"); 

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
     

	}
}
