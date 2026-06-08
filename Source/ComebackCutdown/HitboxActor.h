// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitboxActor.generated.h"

UENUM(BlueprintType)
enum class EHitboxEnum : uint8
{
	HB_STRIKE		UMETA(DisplayName = "Strike"),
	HB_HURTBOX		UMETA(DisplayName = "Hurtbox")
};

UCLASS()
class COMEBACKCUTDOWN_API AHitboxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitboxActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// The damage this hitbox will do.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float hitboxDamage;

	// The distance this attack will knock back.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float launchAmount;

	// The angle this attack will knock back.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float hitboxAngle;

	// The time the character will be stunned.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float stunTime;

	// Damage modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float damageMultiplier;

	// Launch modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float launchMultiplier;

	// Launch modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		bool shouldLaunchMatchCharacterDirection;

	// The location this hitbox will spawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		FVector hitboxLocation;

	// The type of this hitbox.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		EHitboxEnum hitboxType;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
