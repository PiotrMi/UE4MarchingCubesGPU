// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "MarchingCubesComputeShader.h"
#include "MarchingCubesComputeHelper.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShaderExecutionCompleted);

USTRUCT(BlueprintType)
struct FVertex
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Position;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Normal;
};

USTRUCT(BlueprintType)
struct FTriangle
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVertex Vertex1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVertex Vertex2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVertex Vertex3;
};

UCLASS(Blueprintable, BlueprintType)
class MARCHINGCUBESCOMPUTESHADER_API UMarchingCubesComputeHelper : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	~UMarchingCubesComputeHelper();

	UFUNCTION(BlueprintCallable)
		void ExecuteShader();

	void ExecuteShaderInternal();
	void RetrieveDataInternal();

	UFUNCTION(BlueprintCallable)
		TArray<FTriangle> GetMeshData()
	{
		return TArray<FTriangle>(_meshData);
	}

	UPROPERTY(BlueprintAssignable)
		FShaderExecutionCompleted OnShaderExecutionCompleted;
private:
	bool _isExecuting = false;
	bool _isUnloading = false;

	FMarchingCubesComputeShaderConstantParameters _constantParameters;
	FMarchingCubesComputeShaderVariableParameters _variableParameters;
	ERHIFeatureLevel::Type _featureLevel;

	TArray<FTriangle> _meshData;
	FStructuredBufferRHIRef _meshDataBuffer;
	FUnorderedAccessViewRHIRef _meshDataUAV;
};