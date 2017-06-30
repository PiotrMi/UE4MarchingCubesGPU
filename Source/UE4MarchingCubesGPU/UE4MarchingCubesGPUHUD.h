// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "UE4MarchingCubesGPUHUD.generated.h"

UCLASS()
class AUE4MarchingCubesGPUHUD : public AHUD
{
	GENERATED_BODY()

public:
	AUE4MarchingCubesGPUHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

