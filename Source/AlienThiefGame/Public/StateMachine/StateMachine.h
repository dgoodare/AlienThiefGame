// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"

#include "../EnemyCharacterController.h"
#include "Task.h"

#include "StateMachine.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALIENTHIEFGAME_API UStateMachine : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateMachine();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//variable to store refrence to parent object (in this case an EnemyCharacterController)
	//* Enemy = GetOwner();

	//Enumeration of the different states the AI Enemy can be in
	enum EnemyStates {SELECT_TASK, PERFORM_TASK, SEARCH, CHASE};
	EnemyStates State = EnemyStates::SELECT_TASK;

	//Enumeration that dictates whether we are transitioning between states or executing the action in a particular state
	enum EnemyEvents {ON_ENTER, ON_UPDATE};
	EnemyEvents Event = EnemyEvents::ON_ENTER;

	//function that controls whether to change state or to continue in the same state
	void ControlFSM();

	//function that is called when enemy exits a state and enters a new one
	void SetState(EnemyStates NewState);

	/*Each State in the FSM has three functions: Enter, Execute and Exit*/
	//SELECT_TASK
	void SELECT_TASK_Enter();
	void SELECT_TASK_Execute();
	void SELECT_TASK_Exit();
	
	//PERFORM_TASK
	void PERFORM_TASK_Enter();
	void PERFORM_TASK_Execute();
	void PERFORM_TASK_Exit();

	//SEARCH
	void SEARCH_Enter();
	void SEARCH_Execute();
	void SEARCH_Exit();

	//CHASE
	void CHASE_Enter();
	void CHASE_Execute();
	void CHASE_Exit();
};
