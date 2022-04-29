// Fill out your copyright notice in the Description page of Project Settings.


#include "Interact/HidingPlace.h"

// Sets default values
AHidingPlace::AHidingPlace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AHidingPlace::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHidingPlace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHidingPlace::OnInteract_Implementation(AActor* Caller)
{
}

void AHidingPlace::StartFocus_Implementation()
{
}

void AHidingPlace::EndFocus_Implementation()
{
}

