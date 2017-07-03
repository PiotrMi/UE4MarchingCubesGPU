// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GlobalShader.h"
#include "UniformBuffer.h"
#include "RHICommandList.h"

BEGIN_UNIFORM_BUFFER_STRUCT(FMarchingCubesComputeShaderConstantParameters, )
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, IsoLevel)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, GridSize)
END_UNIFORM_BUFFER_STRUCT(FMarchingCubesComputeShaderConstantParameters)

BEGIN_UNIFORM_BUFFER_STRUCT(FMarchingCubesComputeShaderVariableParameters, )
//DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, TotalTimeElapsedSeconds)
END_UNIFORM_BUFFER_STRUCT(FMarchingCubesComputeShaderVariableParameters)

typedef TUniformBufferRef<FMarchingCubesComputeShaderConstantParameters> FMarchingCubesComputeShaderConstantParametersRef;
typedef TUniformBufferRef<FMarchingCubesComputeShaderVariableParameters> FMarchingCubesComputeShaderVariableParametersRef;

class FMarchingCubesComputeShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FMarchingCubesComputeShader, Global);
public:
	FMarchingCubesComputeShader() {};
	explicit FMarchingCubesComputeShader(const ShaderMetaType::CompiledShaderInitializerType& initializer);

	static bool ShouldCache(EShaderPlatform platform)
	{
		return IsFeatureLevelSupported(platform, ERHIFeatureLevel::SM5);
	}

	static void ModifyCompilationEnvironment(EShaderPlatform platform, FShaderCompilerEnvironment& outEnvironment);

	bool Serialize(FArchive& archive) override
	{
		bool parametersOutdated = FGlobalShader::Serialize(archive);
		archive << _meshData;
		return parametersOutdated;
	}

	void SetMeshData(FRHICommandList& rhiCmdList, FUnorderedAccessViewRHIRef meshDataUAV);
	void SetUniformBuffers(FRHICommandList& rhiCmdList, FMarchingCubesComputeShaderConstantParameters& constantParameters, FMarchingCubesComputeShaderVariableParameters& variableParameters);
	void UnbindBuffers(FRHICommandList& rhiCmdList);

private:
	FShaderResourceParameter _meshData;
};