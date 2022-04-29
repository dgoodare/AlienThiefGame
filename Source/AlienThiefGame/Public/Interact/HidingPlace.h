// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InteractInterface.h"

#include "HidingPlace.generated.h"

UCLASS()
class ALIENTHIEFGAME_API AHidingPlace : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHidingPlace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//function that is called when the object is interacted with
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller);

	//function that is called when an interactable object is 'looked' at i.e. to highligh that an object can be interacted with
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void StartFocus();
	virtual void StartFocus_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void EndFocus();
	virtual void EndFocus_Implementation();
};
