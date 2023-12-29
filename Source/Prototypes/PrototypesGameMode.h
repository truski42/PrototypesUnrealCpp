// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PrototypesGameMode.generated.h"

UCLASS(minimalapi)
class APrototypesGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	APrototypesGameMode();

	//Save game funciton
	void SaveGame();

	//Load game function
	void LoadGame();

	//Create save file
	void CreateSaveFile();
};