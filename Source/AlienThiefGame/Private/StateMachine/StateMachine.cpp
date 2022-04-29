// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/StateMachine.h"

// Sets default values for this component's properties
UStateMachine::UStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UStateMachine::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UStateMachine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Call the control function every frame
	ControlFSM();
}

//This function controls the flow between states. It is responsible for transitioning between states as well as staying in the current state
void UStateMachine::ControlFSM()
{
	//SELECT_TASK
	if (State == EnemyStates::SELECT_TASK)
	{
		if (Event == EnemyEvents::ON_ENTER)
		{
			//enter the state
			SELECT_TASK_Enter();
		}
		if (Event == EnemyEvents::ON_UPDATE)
		{
			//execute the functionality of the state
			SELECT_TASK_Execute();
		}
	}

	//PERFORM_TASK
	if (State == EnemyStates::PERFORM_TASK)
	{
		if (Event == EnemyEvents::ON_ENTER)
		{
			//enter the state
			PERFORM_TASK_Enter();
		}
		if (Event == EnemyEvents::ON_UPDATE)
		{
			//execute the functionality of the state
			PERFORM_TASK_Execute();
		}
	}

	//SEARCH
	if (State == EnemyStates::SEARCH)
	{
		if (Event == EnemyEvents::ON_ENTER)
		{
			//enter the state
			SEARCH_Enter();
		}
		if (Event == EnemyEvents::ON_UPDATE)
		{
			//execute the functionality of the state
			SEARCH_Execute();
		}
	}

	//CHASE
	if (State == EnemyStates::CHASE)
	{
		if (Event == EnemyEvents::ON_ENTER)
		{
			//enter the state
			CHASE_Enter();
		}
		if (Event == EnemyEvents::ON_UPDATE)
		{
			//execute the functionality of the state
			CHASE_Execute();
		}
	}
}

void UStateMachine::SetState(EnemyStates NewState)
{
	//switch statement for exiting a given state
	switch (State)
	{
		case EnemyStates::SELECT_TASK:
			SELECT_TASK_Exit();
			break;
		case EnemyStates::PERFORM_TASK:
			PERFORM_TASK_Exit();
			break;
		case EnemyStates::SEARCH:
			SEARCH_Exit();
			break;
		case EnemyStates::CHASE:
			CHASE_Exit();
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Unexpected state!"), NewState);
			return;
	}

	//Assign the current state to the new state
	State = NewState;
	//Set the event to Enter
	Event = EnemyEvents::ON_ENTER;
}


/*----- FUNCTIONS FOR ENTERING, EXECUTING AND EXITING EACH STATE -----*/

//SELECT_TASK
void UStateMachine::SELECT_TASK_Enter()
{
	//Set the Event to ON_UPDATE to call the Execute() function of this state
	UE_LOG(LogTemp, Warning, TEXT("Entering SELECT_TASK"));

	//Get parent class of the state machine component and cast to the correct object type
	AActor* Owner = GetOwner();
	AEnemyCharacterController* Enemy = Cast<AEnemyCharacterController>(Owner);

	//check that the cast was successful
	if (Enemy)
	{
		auto index = FMath::RandRange(0, Enemy->Tasks.Num() - 1);

		ATask* CurrentTask = Cast<ATask>(Enemy->Tasks[index]);

		//update enemy's current task
		Enemy->CurrentTask = CurrentTask;

		//Move enemy to Task location
		Enemy->MoveToActor(Enemy->CurrentTask);
		/*Once enemy arrives at the location, it will trigger an OnMoveComplete event which 
		has an overriden method in EnemyCharacterController that will handle the transition between 
		Entering and Executing behaviour in the SELECT_TASK state.*/

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast to From AActor to AEnemyCharacterController failed"));
	}
}

void UStateMachine::SELECT_TASK_Execute()
{
	/*Select a new task from task list and navigate to task location*/
	UE_LOG(LogTemp, Warning, TEXT("Executing SELECT_TASK"));
	//Get parent class of the state machine component and cast to the correct object type
	AActor* Owner = GetOwner();
	AEnemyCharacterController* Enemy = Cast<AEnemyCharacterController>(Owner);

	//check that the cast was successful
	if (Enemy)
	{
		//transition to the PERFORM_TASK state
		SetState(EnemyStates::PERFORM_TASK);
		//Control flow into the new state
		ControlFSM();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast to From AActor to AEnemyCharacterController failed"));
	}

}

void UStateMachine::SELECT_TASK_Exit()
{
}

//PERFORM_TASK
void UStateMachine::PERFORM_TASK_Enter()
{
	//Set the Event to ON_UPDATE to call the Execute() function of this state
	UE_LOG(LogTemp, Warning, TEXT("Entering PERFORM_TASK"));
	Event = EnemyEvents::ON_UPDATE;
	ControlFSM();
}

void UStateMachine::PERFORM_TASK_Execute()
{
	/*Wait at task location for period of time. Adding randomness at this stage might be beneficial*/
	UE_LOG(LogTemp, Warning, TEXT("Executing PERFORM_TASK"));
	//Get parent class of the state machine component and cast to the correct object type
	AActor* Owner = GetOwner();
	AEnemyCharacterController* Enemy = Cast<AEnemyCharacterController>(Owner);
	
	//check if cast was successful
	if (Enemy)
	{
		

		float Min = Enemy->CurrentTask->MinWaitTime;
		float Max = Enemy->CurrentTask->MaxWaitTime;

		//assign random time for enemy to wait at task location
		auto WaitTime = FMath::RandRange(Min, Max);
		UE_LOG(LogTemp, Warning, TEXT("Waiting for %f seconds"), WaitTime);

		FTimerHandle Timer;

		/*Would like to add a feature that gets the enemy to 'say' something. Which would better telgraph
		to the player the behaviour of the AI*/

		SetState(EnemyStates::SELECT_TASK);

		Enemy->GetWorldTimerManager().SetTimer(Timer, Enemy, &AEnemyCharacterController::HandleTaskTimer, WaitTime, false);

		//transtion back to the SELECT_TASK state
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast to From AActor to AEnemyCharacterController failed"));
	}
}

void UStateMachine::PERFORM_TASK_Exit()
{
}

//SEARCH
void UStateMachine::SEARCH_Enter()
{
	//Set the Event to ON_UPDATE to call the Execute() function of this state
	UE_LOG(LogTemp, Warning, TEXT("Entering SEARCH"));

	//Get parent class of the state machine component and cast to the correct object type
	AActor* Owner = GetOwner();
	AEnemyCharacterController* Enemy = Cast<AEnemyCharacterController>(Owner);

	//check that the cast was successful
	if (Enemy)
	{
		//get the total number of waypoints in the level
		int TotalWaypoints = Enemy->Waypoints.Num() - 1;
		//assign a random value for the number of waypoints the enemy will visit during its SEARCH state
		int NumToVisit = FMath::RandRange(2, TotalWaypoints);

		//boolean that determines if the list is complete
		bool IsComplete = false;

		while (!IsComplete)
		{
			//get a random waypoint from the list of all waypoints
			AWaypoint* RandomWaypoint = Enemy->GetRandomWaypoint();

			//check that waypoint isn't already in the list
			for (int i = 0; i < NumToVisit; i++)
			{
				ToVisitList.AddUnique(RandomWaypoint);
			}

			//check if the list is complete
			if (ToVisitList.Num() == NumToVisit)
			{
				IsComplete = true;
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Array of size %d created"), NumToVisit);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast to From AActor to AEnemyCharacterController failed"));
	}

	//reset visited waypoints counter
	Visited = 0;
	//Update event
	Event = EnemyEvents::ON_UPDATE;
}

void UStateMachine::SEARCH_Execute()
{
	/*Explore non-specific locations in the level to incinuate that the enemy is unaware of the exact location of the player*/
	//Get parent class of the state machine component and cast to the correct object type
	AActor* Owner = GetOwner();
	AEnemyCharacterController* Enemy = Cast<AEnemyCharacterController>(Owner);

	//check that the cast was successful
	if (Enemy)
	{
		Enemy->MoveToActor(ToVisitList[Visited]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast to From AActor to AEnemyCharacterController failed"));
	}
}

void UStateMachine::SEARCH_Exit()
{
}

//CHASE
void UStateMachine::CHASE_Enter()
{
	//Set the Event to ON_UPDATE to call the Execute() function of this state
	UE_LOG(LogTemp, Warning, TEXT("Entering CHASE"));
	Event = EnemyEvents::ON_UPDATE;
}

void UStateMachine::CHASE_Execute()
{
	/*Pathfind towards the player, if they have been in (a short) range for a few seconds, capture them*/
	//Get parent class of the state machine component and cast to the correct object type
	AActor* Owner = GetOwner();
	AEnemyCharacterController* Enemy = Cast<AEnemyCharacterController>(Owner);

	//check if cast was successful
	if (Enemy)
	{
		

		//check if distance is small enough
		/*if (Enemy->GetPawn()->GetDistanceTo(Enemy->ThePlayer) < 125)
		{
			//set timer for 3 seconds in separate thread

			UE_LOG(LogTemp, Warning, TEXT("CAPTURED!"));
		}*/

		//move towards the player
		Enemy->MoveToActor(Enemy->ThePlayer);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Cast to From AActor to AEnemyCharacterController failed"));
	}
}

void UStateMachine::CHASE_Exit()
{
}

