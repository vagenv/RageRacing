// Copyright 2015-2016 Vagen Ayrapetyan

using UnrealBuildTool;
using System.Collections.Generic;

public class RageEditorTarget : TargetRules
{
	public RageEditorTarget(TargetInfo Target)
	{
		Type = TargetType.Editor;
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
