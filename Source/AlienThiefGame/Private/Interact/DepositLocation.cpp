// Fill out your copyright notice in the Description page of Project Settings.


#include "Interact/DepositLocation.h"

// Sets default values
ADepositLocation::ADepositLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADepositLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADepositLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Behviour when this object is interacted with
void ADepositLocation::OnInteract_Implementation(AActor* Caller)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Deposited Item"));
}

void ADepositLocation::StartFocus_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Start Focus"));
	UE_LOG(LogTemp, Warning, TEXT("Start Focus"));
}

void ADepositLocation::EndFocus_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("End Focus"));
	UE_LOG(LogTemp, Warning, TEXT("End Focus"));
}