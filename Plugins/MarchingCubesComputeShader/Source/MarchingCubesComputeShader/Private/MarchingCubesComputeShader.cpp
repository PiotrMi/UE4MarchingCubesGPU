// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MarchingCubesComputeShaderPrivatePCH.h"
#include "ShaderParameterUtils.h"
#include "RHIStaticStates.h"

IMPLEMENT_UNIFORM_BUFFER_STRUCT(FMarchingCubesComputeShaderConstantParameters, TEXT("CSConstants"))
IMPLEMENT_UNIFORM_BUFFER_STRUCT(FMarchingCubesComputeShaderVariableParameters, TEXT("CSVariables"))

FMarchingCubesComputeShader::FMarchingCubesComputeShader(const ShaderMetaType::CompiledShaderInitializerType& initializer) : FGlobalShader(initializer)
{
	_meshData.Bind(initializer.ParameterMap, TEXT("MeshData"));
}

void FMarchingCubesComputeShader::ModifyCompilationEnvironment(EShaderPlatform platform, FShaderCompilerEnvironment& outEnvironment)
{
	FGlobalShader::ModifyCompilationEnvironment(platform, outEnvironment);
	outEnvironment.CompilerFlags.Add(CFLAG_StandardOptimization);
}

void FMarchingCubesComputeShader::SetMeshData(FRHICommandList& rhiCmdList, FUnorderedAccessViewRHIRef meshDataUAV)
{
	FComputeShaderRHIParamRef computeShaderRHI = GetComputeShader();

	if (_meshData.IsBound())
	{
		rhiCmdList.SetUAVParameter(computeShaderRHI, _meshData.GetBaseIndex(), meshDataUAV);
	}
}

void FMarchingCubesComputeShader::SetUniformBuffers(FRHICommandList& rhiCmdList, FMarchingCubesComputeShaderConstantParameters& constantParameters, FMarchingCubesComputeShaderVariableParameters& variableParameters)
{
	FMarchingCubesComputeShaderConstantParametersRef constantParametersBuffer = FMarchingCubesComputeShaderConstantParametersRef::CreateUniformBufferImmediate(constantParameters, UniformBuffer_SingleDraw);
	FMarchingCubesComputeShaderVariableParametersRef variableParametersBuffer = FMarchingCubesComputeShaderVariableParametersRef::CreateUniformBufferImmediate(variableParameters, UniformBuffer_SingleDraw);

	SetUniformBufferParameter(rhiCmdList, GetComputeShader(), GetUniformBufferParameter<FMarchingCubesComputeShaderConstantParameters>(), constantParametersBuffer);
	SetUniformBufferParameter(rhiCmdList, GetComputeShader(), GetUniformBufferParameter<FMarchingCubesComputeShaderVariableParameters>(), variableParametersBuffer);
}

void FMarchingCubesComputeShader::UnbindBuffers(FRHICommandList& rhiCmdList)
{
	FComputeShaderRHIParamRef computeShaderRHI = GetComputeShader();

	if (_meshData.IsBound())
		rhiCmdList.SetUAVParameter(computeShaderRHI, _meshData.GetBaseIndex(), FUnorderedAccessViewRHIRef());
}

IMPLEMENT_SHADER_TYPE(, FMarchingCubesComputeShader, TEXT("MarchingCubesComputeShader"), TEXT("MarchingCubes"), SF_Compute);

IMPLEMENT_MODULE(FDefaultModuleImpl, MarchingCubesComputeShader)