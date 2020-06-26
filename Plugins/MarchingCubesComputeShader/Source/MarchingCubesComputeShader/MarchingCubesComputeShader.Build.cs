// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MarchingCubesComputeShader :ModuleRules
{
    public MarchingCubesComputeShader(ReadOnlyTargetRules Target) : base(Target)
    {

        PublicIncludePaths.AddRange(
            new string[] {
                "MarchingCubesComputeShader/Public"
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "MarchingCubesComputeShader/Private",
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "RenderCore",
                "ShaderCore",
                "RHI"
            }
            );
    }
}
