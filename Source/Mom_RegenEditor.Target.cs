// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Mom_RegenEditorTarget : TargetRules
{
	public Mom_RegenEditorTarget(TargetInfo Target)
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
		OutExtraModuleNames.AddRange( new string[] { "Mom_Regen" } );
	}
}
