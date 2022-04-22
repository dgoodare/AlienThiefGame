// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterController.h"
#include "EnemyCharacter.h"

#include "Waypoint.h"
#include "Task.h"
#include "PlayerCharacter.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AEnemyCharacterController::AEnemyCharacterController()
{
	PrimaryActorTick.bCanEverTick = true;

	//Setup Perception Component
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = AILoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
	SightConfig->SetMaxAge(AISightAge);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyCharacterController::OnPawnDetected);
	GetPerceptionComponent()->ConfigureSense(*SightConfig);

	//Setup State Machine Component
	FSM = CreateDefaultSubobject<UStateMachine>(TEXT("State Machine"));
}

void AEnemyCharacterController::BeginPlay()
{
	Super::BeginPlay();
	
	//setup perception
	if (GetPerceptionComponent() != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("All Systems Set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("A problem occured"));
	}

	//setup list of waypoints
	TSubclassOf<AWaypoint> WaypointToFind;
	WaypointToFind = AWaypoint::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), WaypointToFind, Waypoints);

	//initialise list of all task locations
	TSubclassOf<ATask> TaskToFind;
	TaskToFind = ATask::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TaskToFind, Tasks);

	GoToRandomWaypoint();

}

/*void AEnemyCharacterController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);
}*/

void AEnemyCharacterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (DistanceToPlayer > AISightRadius)
	{
		bIsPlayerDetected = false;
	}
}

FRotator AEnemyCharacterController::GetControlRotation() const
{
	if (GetPawn() == nullptr)
	{
		return FRotator(0.0f, 0.0f, 0.0f);
	}
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

void AEnemyCharacterController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (size_t i = 0; i < DetectedPawns.Num(); i++)
	{
		DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);
		//log for checking purposes
		UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
	}

	bIsPlayerDetected = true;
}

void AEnemyCharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	//once a 'move' is complete, wait for and
	Super::OnMoveCompleted(RequestID, Result);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyCharacterController::GoToRandomWaypoint, 1.0f, false);
	
}

AWaypoint* AEnemyCharacterController::GetRandomWaypoint()
{
	//get number of waypoints in the list
	auto index = FMath::RandRange(0, Waypoints.Num() - 1);

	return Cast<AWaypoint>(Waypoints[index]);
}

void AEnemyCharacterController::GoToRandomWaypoint()
{
	if (bIsPlayerDetected == true)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		MoveToActor(Player, 5.0f);
	}
	else
	{
		MoveToActor(GetRandomWaypoint());
	}
}

void AEnemyCharacterController::DoNothing()
{

}
