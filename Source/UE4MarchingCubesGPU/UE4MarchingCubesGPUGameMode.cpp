// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "UE4MarchingCubesGPU.h"
#include "UE4MarchingCubesGPUGameMode.h"
#include "UE4MarchingCubesGPUHUD.h"
#include "UE4MarchingCubesGPUCharacter.h"

AUE4MarchingCubesGPUGameMode::AUE4MarchingCubesGPUGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUE4MarchingCubesGPUHUD::StaticClass();
}
