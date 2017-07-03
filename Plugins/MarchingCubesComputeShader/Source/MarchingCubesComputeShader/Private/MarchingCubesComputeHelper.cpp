// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MarchingCubesComputeShaderPrivatePCH.h"

UMarchingCubesComputeHelper::UMarchingCubesComputeHelper(const class FObjectInitializer& initializer)
{
	UObject* outer = GetOuter();
	if (outer && outer->ImplementsGetWorld())
	{
		_featureLevel = outer->GetWorld()->Scene->GetFeatureLevel();
	}
	_constantParameters.GridSize = 1;
	_constantParameters.IsoLevel = 1;

	_variableParameters = FMarchingCubesComputeShaderVariableParameters();

	//FRHIResourceCreateInfo CreateInfo;
}

UMarchingCubesComputeHelper::~UMarchingCubesComputeHelper()
{
	_isUnloading = true;
}

void UMarchingCubesComputeHelper::ExecuteShader()
{
	if (_isUnloading || _isExecuting)
	{
		return;
	}

	_isExecuting = true;

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		FComputeShaderRunner,
		UMarchingCubesComputeHelper*,
		computeShader,
		this,
		{
			computeShader->ExecuteShaderInternal();
		}
	)
}

void UMarchingCubesComputeHelper::ExecuteShaderInternal()
{
	check(IsInRenderingThread());

	if (_isUnloading)
	{
		if (_meshDataUAV != NULL)
		{
			_meshDataUAV.SafeRelease();
			_meshDataUAV = NULL;
		}
		return;
	}

	FRHICommandListImmediate& rhiCmdList = GRHICommandList.GetImmediateCommandList();

	TShaderMapRef<FMarchingCubesComputeShader> computeShader(GetGlobalShaderMap(_featureLevel));
	rhiCmdList.SetComputeShader(computeShader->GetComputeShader());

	computeShader->SetMeshData(rhiCmdList, _meshDataUAV);
	computeShader->SetUniformBuffers(rhiCmdList, _constantParameters, _variableParameters);

	DispatchComputeShader(rhiCmdList, *computeShader, 8, 8, 8);

	computeShader->UnbindBuffers(rhiCmdList);

	_isExecuting = false;
}