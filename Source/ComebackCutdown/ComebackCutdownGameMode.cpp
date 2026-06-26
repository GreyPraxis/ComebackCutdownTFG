// Copyright Epic Games, Inc. All Rights Reserved.

#include "ComebackCutdownGameMode.h"
#include "ComebackCutdownCharacter.h"
#include "UObject/ConstructorHelpers.h"

AComebackCutdownGameMode::AComebackCutdownGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	numRounds = 2;
	roundTime = 99;
	isTimerActive = true;
}
