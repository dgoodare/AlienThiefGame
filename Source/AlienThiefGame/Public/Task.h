// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Waypoint.h"
#include "Task.generated.h"

/**
 * 
 */
UCLASS()
class ALIENTHIEFGAME_API ATask : public AWaypoint
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MinWaitTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxWaitTime;

};
