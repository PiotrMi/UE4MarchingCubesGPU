// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4MarchingCubesGPUEditorTarget : TargetRules
{
	public UE4MarchingCubesGPUEditorTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.Add("UE4MarchingCubesGPU");
	}

	//
	// TargetRules interface.
	//
}
