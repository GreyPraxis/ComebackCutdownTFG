// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComebackCutdownCharacter.h"
#include "EsperantoCharacter.generated.h"

/**
 * 
 */
UCLASS()
class COMEBACKCUTDOWN_API AEsperantoCharacter : public AComebackCutdownCharacter
{
	GENERATED_BODY()


	//void AttackWeak() override;
	//void AttackNormal() override;

protected: 


	//Saltos
	//void StartJump();
	//void StopJump();

	//Correr
	//void Sprint();

	/** Called for side to side input */
	//void MoveRight(float Val);
	//void UpDown(float Val);

	//Antigua revisión de salto
	/** Handle touch inputs. */
	//void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);
	/** Handle touch stop event. */
	//void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);


	// Damage the character
	//void TakeDamage(float _damageAmount);


public:
	AEsperantoCharacter();
};
