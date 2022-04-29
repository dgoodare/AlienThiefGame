// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterController.h"
#include "EnemyCharacter.h"

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
		UE_LOG(LogTemp, Warning, TEXT("AI perception component successfully created"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("A problem occured creating AI perception"));
	}

	//get reference to player
	ThePlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (ThePlayer == nullptr)//check that the cast did not return null
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get reference to Player"));
	}

	//setup list of waypoints
	TSubclassOf<AWaypoint> WaypointToFind;
	WaypointToFind = AWaypoint::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), WaypointToFind, Waypoints);

	//initialise list of all task locations
	TSubclassOf<ATask> TaskToFind;
	TaskToFind = ATask::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TaskToFind, Tasks);

	if (FSM != nullptr)
	{
		FSM->ControlFSM();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get reference to State machine"));
	}
	

}

/*void AEnemyCharacterController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);
}*/

void AEnemyCharacterController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (ThePlayer->IsHiding && FSM->State == FSM->EnemyStates::CHASE)
	{
		FSM->EnemyEvents::ON_ENTER;
		FSM->SetState(FSM->EnemyStates::SEARCH);
		FSM->ControlFSM();
		ThePlayer->StateIcon(1);
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

	
	if (!ThePlayer->IsHiding && FSM->State != FSM->EnemyStates::CHASE)
	{
		FSM->EnemyEvents::ON_ENTER;
		FSM->SetState(FSM->EnemyStates::CHASE);
		FSM->ControlFSM();
		ThePlayer->StateIcon(2);
	}
	else
	{
		FSM->EnemyEvents::ON_ENTER;
		FSM->SetState(FSM->EnemyStates::SEARCH);
		FSM->ControlFSM();
		ThePlayer->StateIcon(1);
	}
	
}

void AEnemyCharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	//once a 'move' is complete, wait for and
	Super::OnMoveCompleted(RequestID, Result);

	//GetWorldTimerManager().SetTimer(TimerHandle, this, &AEnemyCharacterController::GoToRandomWaypoint, 1.0f, false);

	/*It would have been cleaner to write the following section as a switch statement, but c++ does not
	allow you to do this dynamically (as is required in this case)*/

	if (FSM->State == FSM->EnemyStates::SELECT_TASK && FSM->Event == FSM->EnemyEvents::ON_ENTER)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task location reached, entering new state"));
		//Update FSM variables to transtion into the PERFROM_TASK state
		FSM->EnemyEvents::ON_ENTER;
		FSM->SetState(FSM->EnemyStates::PERFORM_TASK);
		FSM->ControlFSM();
		ThePlayer->StateIcon(0);
	}
	else if (FSM->State == FSM->EnemyStates::SEARCH)
	{
		FSM->Visited++;
		//check if all the waypoints have been visited
		if ( FSM->Visited < FSM->ToVisitList.Num() )
		{
			//increment visited counter
			UE_LOG(LogTemp, Warning, TEXT("Visited %d waypoints"), FSM->Visited);
			
			//continue searching
			FSM->ControlFSM();
		}
		else
		{
			//exit SEARCH state and enter SELECT_TASK
			FSM->EnemyEvents::ON_ENTER;
			FSM->SetState(FSM->EnemyStates::SELECT_TASK);
			FSM->ControlFSM();
			ThePlayer->StateIcon(0);
		}
		
	}
	else if (FSM->State == FSM->EnemyStates::CHASE)
	{
		if (ThePlayer->IsHiding)
		{
			FSM->EnemyEvents::ON_ENTER;
			FSM->SetState(FSM->EnemyStates::SEARCH);
			FSM->ControlFSM();
			ThePlayer->StateIcon(0);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("CAPTURED!"));
		}
	}
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
		
		MoveToActor(ThePlayer, 5.0f);
	}
	else
	{
		MoveToActor(GetRandomWaypoint());
	}
}

void AEnemyCharacterController::HandleTaskTimer()
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("Timer finished"));
	
	if (FSM->State == FSM->EnemyStates::CHASE)
	{
		//We don't want the Enemy changing out of the CHASE state, so stay in the same state
		UE_LOG(LogTemp, Warning, TEXT("Continuing CHASE"));
		FSM->ControlFSM();
	}
	else
	{
		//Update FSM variables to transtion into the PERFROM_TASK state
		UE_LOG(LogTemp, Warning, TEXT("Player not detected staying in Passive mode"));
		FSM->EnemyEvents::ON_UPDATE;
		FSM->SetState(FSM->EnemyStates::SELECT_TASK);
		FSM->ControlFSM();
	}
	*/
	FSM->ControlFSM();
}


int AEnemyCharacterController::UpdateStateIcon_Implementation()
{
	if (FSM->State == FSM->EnemyStates::SELECT_TASK || FSM->State == FSM->EnemyStates::PERFORM_TASK)
	{
		return 0;
	}
	else if (FSM->State == FSM->EnemyStates::SEARCH)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}