// Fill out your copyright notice in the Description page of Project Settings.


#include "Interact/InteractBase.h"

// Sets default values
AInteractBase::AInteractBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractBase::OnInteract_Implementation(AActor* Caller)
{
	Destroy();
}

void AInteractBase::StartFocus_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Start Focus"));
	UE_LOG(LogTemp, Warning, TEXT("Start Focus"));
}

void AInteractBase::EndFocus_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("End Focus"));
	UE_LOG(LogTemp, Warning, TEXT("End Focus"));
}
