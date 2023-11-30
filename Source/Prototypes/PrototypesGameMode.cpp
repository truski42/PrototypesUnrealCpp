// Copyright Epic Games, Inc. All Rights Reserved.

#include "PrototypesGameMode.h"

#include "EngineUtils.h"
#include "PrototypesCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Core/PrototypeSaveGame.h"
#include "Core/PrototypeGameplayInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

APrototypesGameMode::APrototypesGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	LoadGame();

}
// Create save file
void APrototypesGameMode::CreateSaveFile()
{
	UPrototypeSaveGame* DataToSave = Cast<UPrototypeSaveGame>(UGameplayStatics::CreateSaveGameObject(UPrototypeSaveGame::StaticClass()));
	UGameplayStatics::SaveGameToSlot(DataToSave, "Slot1", 0);
}

void APrototypesGameMode::SaveGame()
{
	UPrototypeSaveGame* DataToSave = Cast<UPrototypeSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));

	if(DataToSave !=nullptr)
	{
		//DataToSave->SavedPlayer.Empty();
		DataToSave->SavedActors.Empty();
		
		for(FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->Implements<UPrototypeGameplayInterface>())
			{
				UE_LOG(LogTemp, Warning, TEXT("Name of Actor Saved: %s"), *Actor->GetFName().ToString());
				FActorSaveData ActorSaveData;
				ActorSaveData.ActorName = Actor->GetFName();
				ActorSaveData.Rotation = Actor->GetActorRotation();
				ActorSaveData.Location = Actor->GetActorLocation();
				FMemoryWriter MemWriter(ActorSaveData.ByteData);

				FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);

				Ar.ArIsSaveGame = true;
				Actor->Serialize(Ar);

				DataToSave->SavedActors.Add(ActorSaveData);
			}
		}
		UGameplayStatics::SaveGameToSlot(DataToSave, "Slot1",0);
	}else if (!UGameplayStatics::DoesSaveGameExist("Slo1", 0))
	{
		CreateSaveFile();
	}
}

void APrototypesGameMode::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist("Slot1", 0)){
		UPrototypeSaveGame* DataToLoad = Cast<UPrototypeSaveGame>(UGameplayStatics::LoadGameFromSlot("Slot1", 0));
		if (DataToLoad == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}
		
		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame Data."));
		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			for (FActorSaveData ActorData : DataToLoad->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetFName())
				{
					Actor->SetActorRotation(ActorData.Rotation);
					Actor->SetActorLocation(ActorData.Location);
					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
					Ar.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					Actor->Serialize(Ar);
					UE_LOG(LogTemp, Warning, TEXT("Name of Actor Loaded: %s"), *Actor->GetFName().ToString());
					UE_LOG(LogTemp, Warning, TEXT("Name of Actor Transform: %s"), *ActorData.Rotation.ToString());

					break;
				}
			}
		}
	}
}


