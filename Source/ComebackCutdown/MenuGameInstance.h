// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ComebackCutdownCharacter.h"
#include "MenuGameInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	VE_Default		UMETA(DisplayName = "Mannequin"),
	VE_Fox			UMETA(DisplayName = "Esperanto")
};

USTRUCT(BlueprintType)
struct FPlayerDetails //Information about the player
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FString playerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		ECharacterClass characterClass;

	// A futuro añadir aquí selección de colores
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool edit1Set;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FVector textureEdit1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		bool edit2Set;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		FVector textureEdit2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		int gamesWon;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	//	FVector textureEdit3;
};




UCLASS()
class COMEBACKCUTDOWN_API UMenuGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	//	int numPlayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		TArray<FPlayerDetails> players;


	// Time for each round
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance Settings")
		float roundTime;

	// Number of rounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance Settings")
		int numRounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mode")
		bool isDeviceUsedForMultiplePlayers;
};
