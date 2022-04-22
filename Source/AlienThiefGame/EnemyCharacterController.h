// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "StateMachine/StateMachine.h"
#include "Waypoint.h"
#include "Task.h"

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

	//function to retrieve a random waypoint
	AWaypoint* GetRandomWaypoint();

	void DoNothing();
	
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

	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = AI)
		bool bIsPlayerDetected = false;//variable that delineates whether the player has been detected

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = AI)
		float DistanceToPlayer = 0.0f;

	//State Manager object
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UStateMachine* FSM;

	//array to store waypoints
	UPROPERTY()
		TArray<AActor*> Waypoints;

	//array to store tasks
	UPROPERTY()
		TArray<AActor*> Tasks;

	UPROPERTY()
		ATask* CurrentTask;

private:

	//function to send the enemy to the waypoint
	UFUNCTION()
		void GoToRandomWaypoint();

	//timer for enemy to wait at waypoint
	FTimerHandle TimerHandle;
};
