// Fill out your copyright notice in the Description page of Project Settings.


#include "PrototypeItem.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
APrototypeItem::APrototypeItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Points = 0;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void APrototypeItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APrototypeItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

