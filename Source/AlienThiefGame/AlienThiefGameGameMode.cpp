// Copyright Epic Games, Inc. All Rights Reserved.

#include "AlienThiefGameGameMode.h"
#include "AlienThiefGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAlienThiefGameGameMode::AAlienThiefGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
