// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MarchingCubesComputeShaderPrivatePCH.h"

#define NUM_TRIANGLES_COUNT 1

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

	// Setup buffers
	_meshData.Init(FTriangle(), 1);
	TResourceArray<FTriangle> bufferData;
	bufferData.Init(FTriangle(), NUM_TRIANGLES_COUNT);
	bufferData.SetAllowCPUAccess(true);
	FRHIResourceCreateInfo createInfo;
	createInfo.ResourceArray = &bufferData;

	_meshDataBuffer = RHICreateStructuredBuffer(sizeof(FTriangle), sizeof(FTriangle) * NUM_TRIANGLES_COUNT, BUF_UnorderedAccess | BUF_ShaderResource, createInfo);
	_meshDataUAV = RHICreateUnorderedAccessView(_meshDataBuffer, true, false);
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

	RetrieveDataInternal();
}

void UMarchingCubesComputeHelper::RetrieveDataInternal()
{
	check(IsInRenderingThread());

	uint8* srcPtr = (uint8*)RHILockStructuredBuffer(_meshDataBuffer, 0, sizeof(FTriangle) * NUM_TRIANGLES_COUNT, EResourceLockMode::RLM_ReadOnly);
	// Reference pointer to first element for our destination ComputedColors
	uint8* dstPtr = (uint8*)_meshData.GetData();
	// Copy from GPU to main memory
	FMemory::Memcpy(dstPtr, srcPtr, sizeof(FTriangle) * NUM_TRIANGLES_COUNT);
	//Unlock texture
	RHIUnlockStructuredBuffer(_meshDataBuffer);

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		_isExecuting = false;
		OnShaderExecutionCompleted.Broadcast();
	}, TStatId(), NULL, ENamedThreads::GameThread);
}