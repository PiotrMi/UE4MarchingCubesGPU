// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4MarchingCubesGPUTarget : TargetRules
{
	public UE4MarchingCubesGPUTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.Add("UE4MarchingCubesGPU");
	}

	//
	// TargetRules interface.
	//


}
