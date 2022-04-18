// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Waypoint.generated.h"

UCLASS()
class ALIENTHIEFGAME_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaypoint();

	//root component
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
		USceneComponent* Root;

	//collision box
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
		UBoxComponent* BoxComponent;

	//reference to the next waypoint in the chain
	UPROPERTY(EditAnywhere, BluePrintReadWrite)
		AWaypoint* NextWaypoint;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnPlayerEnter(UPrimitiveComponent* OverlapComponent,
						   AActor* OtherActor,
						   UPrimitiveComponent* OtherComponent,
						   int32 OtherBodyIndex,
						   bool bFromSweep,
						   const FHitResult& SweepResult);
};
