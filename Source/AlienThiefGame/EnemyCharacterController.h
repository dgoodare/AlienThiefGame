// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Waypoint.h"
#include "EnemyCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class ALIENTHIEFGAME_API AEnemyCharacterController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyCharacterController();//constructor

	virtual void BeginPlay() override;

	//virtual void Possess(APawn* Pawn) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	
	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	//Vision Cone variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 500.0f;//enemy will 'notice' the player if they enter this area
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 5.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseSightRadius = AISightRadius + 50.0f;// enemy will 'lose sight' of player if they leave this area
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 90.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* SightConfig;

	//Hearing circle variables
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIHearingRadius = 50.0f;//enemy will 'notice' the player if they enter this area
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIHearingAge = 5.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AILoseHearingRadius = AISightRadius + 50.0f;//enemy will 'stop hearing' player if they leave this area
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 360.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAIHearingConfig_Sight* HearingConfig;
		*/

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = AI)
		bool bIsPlayerDetected = false;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = AI)
		float DistanceToPlayer = 0.0f;

private:
	//array to store waypoints
	UPROPERTY()
		TArray<AActor*> Waypoints;

	//function to retrieve the next waypoint
	UFUNCTION()
		AWaypoint* GetRandomWaypoint();

	//function to send the enemy to the waypoint
	UFUNCTION()
		void GoToRandomWaypoint();

	//timer for enemy to wait at waypoint
	FTimerHandle TimerHandle;
};
