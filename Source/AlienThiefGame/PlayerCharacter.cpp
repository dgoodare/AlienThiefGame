// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "DrawDebugHelpers.h"
#include "Interact/InteractInterface.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//disable controller rotation pitch, yaw and roll
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//initialise character movement variables
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	//create and intialise camera boom object
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	//set length of camera boom
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	//create and initialise camera object
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//disable pawn control rotation as that is being handled by boom arm
	FollowCamera->bUsePawnControlRotation = false;

	//set default value for trace distance i.e. how close the player needs to be to interact with objects
	TraceDistance = 340.0f;

	//set max inventory capacity
	InventorySize = 2;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForward();
	FocusedActorDistanceCheck();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//setup player input mouse controls
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	//setup player input key controls
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::InteractPressed);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

/* Movement Functions*/
void APlayerCharacter::MoveForward(float Axis)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Axis);
}

void APlayerCharacter::MoveRight(float Axis)
{
	FRotator Rotation = Controller->GetControlRotation();
	FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

	FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Axis);
}

void APlayerCharacter::FocusedActorDistanceCheck()
{
	if (FocusedActor)
	{
		float Distance = (this->GetActorLocation() - FocusedActor->GetActorLocation()).Size();

		if (Distance > TraceDistance)
		{
			IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
			if (Interface)
			{
				Interface->Execute_EndFocus(FocusedActor);
			}
			FocusedActor = nullptr;
		}
	}
}

void APlayerCharacter::TraceForward_Implementation()
{
	FVector Location;
	FRotator Rotation;
	FHitResult Hit;

	GetController()->GetPlayerViewPoint(Location, Rotation);

	FVector Start = Location;
	FVector End = Start + (Rotation.Vector() * TraceDistance);

	FCollisionQueryParams TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, TraceParams);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f);

	if (bHit)
	{
		//DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Red, false, 1.0f);

		AActor* Interactable = Hit.GetActor();


		//check if the actor is interactable
		if (Interactable)
		{
			//check if this actor is a new actor
			if (Interactable != FocusedActor)
			{
				//if there was a focused actor already, end focus with that actor
				if (FocusedActor)
				{
					IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
					
					//make sure that the cast was successful, otherwise the game will crash
					if (Interface)
					{
						Interface->Execute_EndFocus(FocusedActor);
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Cast to Interface object failed"));
					}
				}
				//set the focused actor to the new one
				IInteractInterface* Interface = Cast<IInteractInterface>(Interactable);
				if (Interface)
				{
					Interface->Execute_StartFocus(Interactable);
				}
				FocusedActor = Interactable;

			}
		}
		else
		{

			//reset the focused actor
			if (FocusedActor)
			{
				IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
				if (Interface)
				{
					Interface->Execute_EndFocus(FocusedActor);
				}
			}

			FocusedActor = nullptr;
		}
	}
}


/*Interaction Functions*/
void APlayerCharacter::InteractPressed_Implementation()
{
	TraceForward();
	//if an interactable actor is found, execute the interaction
	if (FocusedActor)
	{
		IInteractInterface* Interface = Cast<IInteractInterface>(FocusedActor);
		if (Interface)
		{
			//check if Interface is an 'Item'
			if (AItem* ItemCheck = Cast<AItem>(Interface))
			{
				HandleItem(ItemCheck, Interface);
			}
			//check if Interface is a Deposit Location
			else if (ADepositLocation* Deposit = Cast<ADepositLocation>(Interface))
			{
				HandleDeposit(Deposit, Interface);
			}
			else
			{
				Interface->Execute_OnInteract(FocusedActor, this);
				Interface->Execute_EndFocus(FocusedActor);
			}
		}
	}
}

void APlayerCharacter::HandleItem(AItem* Item, IInteractInterface* Interface)
{
	//if there is enough free space in inventory
	if (Inventory.Num() < InventorySize)
	{
		//check if that object has already been stored
		bool IsInInventory = false;
		for (int i = 0; i < Inventory.Num(); i++)
		{
			if (Item == Inventory[i])
			{
				IsInInventory = true;
			}
		}

		//a bug can occur that can allow the player to pick upan item twice, so need to check that
		//the object reference isn't already in the item list
		if (!IsInInventory)
		{
			//add to inventory
			Interface->Execute_OnInteract(FocusedActor, this);
			Inventory.Add(Item);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Added item to inventory"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Inventory size: %d"), Inventory.Num()));
		}
		else
		{
			//
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Item already in inventory"));
		}
			
	}
	else
	{
		//tell player 
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, TEXT("Inventory full"));
	}	
}

void APlayerCharacter::HandleDeposit(ADepositLocation* Deposit, IInteractInterface* Interface)
{
	if (Inventory.Num() == 0)
	{
		//if there is nothing in the player's inventory
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Inventory empty, nothing to deposit"));
	}
	else
	{
		/*Currently, only one item is deposited at a time. However, this could be changed*/

		AItem* TempItem;
				
		//move the first item from player's inventory to the deposit location's inventory
		TempItem = Inventory[0];
		Deposit->DepositInventory.Add(TempItem);
		Inventory.Remove(TempItem);
		Interface->Execute_OnInteract(FocusedActor, this);
		
	}
}