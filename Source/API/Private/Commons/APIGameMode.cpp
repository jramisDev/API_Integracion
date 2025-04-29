// Copyright Epic Games, Inc. All Rights Reserved.

#include "Commons/APIGameMode.h"
#include "Commons/APICharacter.h"
#include "UObject/ConstructorHelpers.h"

AAPIGameMode::AAPIGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
