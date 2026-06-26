// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ComebackCutdownCharacter.h"
#include "ComebackCutdownGameMode.generated.h"

UCLASS(minimalapi)
class AComebackCutdownGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AComebackCutdownGameMode();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		AComebackCutdownCharacter* player1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		AComebackCutdownCharacter* player2;


	// Time for each round
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemode Settings")
		float roundTime;

	// Number of rounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemode Settings")
		int numRounds;

	// Number of rounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamemode Settings")
		bool isTimerActive;


};



