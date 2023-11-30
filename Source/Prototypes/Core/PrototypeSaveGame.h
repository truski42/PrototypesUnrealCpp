// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PrototypeSaveGame.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FActorSaveData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName ActorName;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FRotator Rotation;
	
	UPROPERTY()
	FVector Location;
	
	UPROPERTY()
	TArray<uint8> ByteData;
	
};

USTRUCT()
struct FPlayerSaveData
{
	GENERATED_BODY()

public:
	FPlayerSaveData()
	{
		Location = FVector::ZeroVector;
		Rotation = FRotator::ZeroRotator;
		bResumeAtTransform = true;
	}

	UPROPERTY()
	FString PlayerID;
	
	/* Location if player was alive during save */
	UPROPERTY()
	FVector Location;

	/* Orientation if player was alive during save */ 
	UPROPERTY()
	FRotator Rotation;

	/* We don't always want to restore location, and may just resume player at specific respawn point in world. */
	UPROPERTY()
	bool bResumeAtTransform;
};


UCLASS()
class PROTOTYPES_API UPrototypeSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FPlayerSaveData> SavedPlayer;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
};
