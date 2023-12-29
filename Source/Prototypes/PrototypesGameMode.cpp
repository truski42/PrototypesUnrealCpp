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
		DataToSave->SavedPlayer.Empty();
		DataToSave->SavedActors.Empty();
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);
		FPlayerSaveData PlayerSaveData;
		FActorSaveData ActorSaveData;
		for(FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (Actor->Implements<UPrototypeGameplayInterface>())
			{
				if(Actor == Player)
				{
					UE_LOG(LogTemp, Warning, TEXT("Name of Player name %s"), *Actor->GetFName().ToString());
					PlayerSaveData.PlayerID = Player->GetFName().ToString();
					PlayerSaveData.Location = Player->GetActorLocation();
					PlayerSaveData.Rotation = Player->GetController()->GetControlRotation();
				}else
				{
					UE_LOG(LogTemp, Warning, TEXT("Name of Actor Saved: %s"), *Actor->GetFName().ToString());
					ActorSaveData.ActorName = Actor->GetFName();
					ActorSaveData.Rotation = Actor->GetActorRotation();
					ActorSaveData.Location = Actor->GetActorLocation();
				}
				FMemoryWriter MemWriter(ActorSaveData.ByteData);
				FMemoryWriter MemWriterPlayer(PlayerSaveData.ByteData);

				FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
				FObjectAndNameAsStringProxyArchive ArPlayer(MemWriterPlayer, true);

				Ar.ArIsSaveGame = true;
				ArPlayer.ArIsSaveGame = true;
				Actor->Serialize(Ar);

				DataToSave->SavedActors.Add(ActorSaveData);
				DataToSave->SavedPlayer.Add(PlayerSaveData);
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
		//UGameplayStatics::OpenLevel((UObject*)GetGameInstance(), FName(TEXT("FirstPersonMap")));
		if (DataToLoad == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame Data."));
			return;
		}
		ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this->GetWorld(), 0);

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			for (FPlayerSaveData PlayerSaveData : DataToLoad->SavedPlayer)
			{
				if(PlayerSaveData.PlayerID == Player->GetFName().ToString())
				{
					Player->SetActorLocation(PlayerSaveData.Location);
					Player->GetController()->SetControlRotation(PlayerSaveData.Rotation);
					FMemoryReader MemReaderPlayer(PlayerSaveData.ByteData);
					FObjectAndNameAsStringProxyArchive ArPlayer(MemReaderPlayer, true);
					ArPlayer.ArIsSaveGame = true;
					// Convert binary array back into actor's variables
					Actor->Serialize(ArPlayer);
				}
			}
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
					break;
				}
			}
		}
	}
}