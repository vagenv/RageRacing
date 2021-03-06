// Copyright 2015-2016 Vagen Ayrapetyan

using UnrealBuildTool;
using System.Collections.Generic;

public class RageTarget : TargetRules
{
	public RageTarget(TargetInfo Target)
	{
		Type = TargetType.Game;
	}

	//
	// TargetRules interface.
	//

	public override void SetupBinaries(
		TargetInfo Target,
		ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
		ref List<string> OutExtraModuleNames
		)
	{
		OutExtraModuleNames.Add("Rage");
	}
}
