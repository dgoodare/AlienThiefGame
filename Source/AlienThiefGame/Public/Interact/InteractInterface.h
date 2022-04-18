// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ALIENTHIEFGAME_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//function that is called when the object is interacted with
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void OnInteract(AActor* Caller);

	//function that is called when an interactable object is 'looked' at i.e. to highligh that an object can be interacted with
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void StartFocus();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interact")
		void EndFocus();
};
