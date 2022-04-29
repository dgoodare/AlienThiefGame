// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interact/Item.h"
#include "Interact/DepositLocation.h"
#include "Interact/HidingPlace.h"

#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ALIENTHIEFGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GUI)
		TArray<AItem*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GUI)
		bool IsInventoryFull = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GUI)
		int InventorySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GUI)
		bool IsHiding = false;

	UFUNCTION(BlueprintNativeEvent)
		void StateIcon(int val);//Hides the player when they interact with a hiding place
		void StateIcon_Implementation(int val);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	UFUNCTION(BluePrintNativeEvent)
		void InteractPressed();
		void InteractPressed_Implementation();

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = "Interact")
		float TraceDistance;

	UFUNCTION(BluePrintNativeEvent)
		void TraceForward();
		void TraceForward_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void HandleHide(AHidingPlace* Hide);//Hides the player when they interact with a hiding place
		void HandleHide_Implementation(AHidingPlace* Hide);

	UFUNCTION(BlueprintNativeEvent)
		void CheckInventory();//Hides the player when they interact with a hiding place
		void CheckInventory_Implementation();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = GUI)
		int IconValue;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	AActor* FocusedActor;//the (interactable) object the player is looking at
	
	void FocusedActorDistanceCheck();//checks if the focused object is still in range

	void HandleItem(AItem* Item, IInteractInterface* Interface);//Handles an item that player interacts with

	void HandleDeposit(ADepositLocation* Deposit, IInteractInterface* Interface);//Handles depositing of items
	
};
