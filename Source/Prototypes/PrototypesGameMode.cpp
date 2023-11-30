// Copyright Epic Games, Inc. All Rights Reserved.

#include "PrototypesGameMode.h"
#include "PrototypesCharacter.h"
#include "UObject/ConstructorHelpers.h"

APrototypesGameMode::APrototypesGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
