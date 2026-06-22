// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "HitboxActor.generated.h"

UENUM(BlueprintType)
enum class EHitboxEnum : uint8
{
	HB_STRIKE		UMETA(DisplayName = "Strike"),
	HB_HURTBOX		UMETA(DisplayName = "Hurtbox")
};

UENUM(BlueprintType)
enum class EHitboxPosition : uint8
{
	HB_AERIAL		UMETA(DisplayName = "Aerial"),
	HB_LOW			UMETA(DisplayName = "Low"),
	HB_NEUTRAL		UMETA(DisplayName = "Neutral"),
	HB_STRONG		UMETA(DisplayName = "Strong")
};

USTRUCT(BlueprintType)
struct FHitboxData : public FTableRowBase
{
	GENERATED_BODY()

public:
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
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		//float damageMultiplier;

	// Launch modifier
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		//float launchMultiplier;

	// Launch modifier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		bool shouldLaunchMatchCharacterDirection;

	// The location this hitbox will spawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		FTransform hitboxTransform;

	// The type of this hitbox.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		EHitboxEnum hitboxType;

	// The type of this hitbox.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		EHitboxPosition hitboxPosition;

	// The type of this hitbox.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		bool shouldAttach;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		FHitboxData hitboxData;

	// The type of this hitbox.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		EHitboxEnum hitboxType;

	// The damage this hitbox will do.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		float hitboxDamage;


	// The location this hitbox will spawn.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hitbox")
		FVector hitboxLocation;

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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
