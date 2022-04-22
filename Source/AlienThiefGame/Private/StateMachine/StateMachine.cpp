// Fill out your copyright notice in the Description page of Project Settings.


#include "StateMachine/StateMachine.h"

// Sets default values for this component's properties
UStateMachine::UStateMachine()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
	Event = EnemyEvents::ON_UPDATE;
}

void UStateMachine::SELECT_TASK_Execute()
{
	/*Select a new task from task list and navigate to task location*/

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
		Enemy->MoveToActor(CurrentTask);

		//transition to the PERFORM_TASK state
		SetState(EnemyStates::PERFORM_TASK);
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
	Event = EnemyEvents::ON_UPDATE;
}

void UStateMachine::PERFORM_TASK_Execute()
{
	/*Wait at task location for period of time. Adding randomness at this stage might be beneficial*/

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

		FTimerHandle Timer;

		/*Would like to add a feature that gets the enemy to 'say' something. Which would better telgraph
		to the player the behaviour of the AI*/

		Enemy->GetWorldTimerManager().SetTimer(Timer, Enemy, &AEnemyCharacterController::DoNothing, WaitTime, false);

		//transtion back to the SELECT_TASK state
		SetState(EnemyStates::SELECT_TASK);
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
		//get the total number of waypoints in the level
		int TotalWaypoints = Enemy->Waypoints.Num() - 1;
		//assign a random value for the number of waypoints the enemy will visit during its SEARCH state
		int NumToVisit = FMath::RandRange(2, TotalWaypoints);

		//create array to store these waypoints
		TArray<AWaypoint*> ToVisitList;

		//boolean that determines if the list is complete
		bool IsComplete = false;

		while(!IsComplete)
		{
			//get a random waypoint from the list of all waypoints
			AWaypoint* RandWaypoint = Enemy->GetRandomWaypoint();

			//check that waypoint isn't already in the list
			for (int i = 0; i < NumToVisit; i++)
			{
				if (RandWaypoint != ToVisitList[i])
				{
					ToVisitList.Add(RandWaypoint);
				}
			}

			//check if the list is complete
			if (ToVisitList.Num() == NumToVisit)
			{
				IsComplete = true;
			}
		}

		//Get Enemy to search in the randomly assigned waypoint locations
		for (int i = 0; i < NumToVisit; i++)
		{
			Enemy->MoveToActor(ToVisitList[i]);
		}

		//transition back to the SELECT_TASK state
		SetState(EnemyStates::SELECT_TASK);
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
	Event = EnemyEvents::ON_UPDATE;
}

void UStateMachine::CHASE_Execute()
{
	/*Pathfind towards the player, if they have been in (a short) range for a few seconds, capture them*/
}

void UStateMachine::CHASE_Exit()
{
}

