// Fill out your copyright notice in the Description page of Project Settings.


#include "HitboxActor.h"

// Sets default values
AHitboxActor::AHitboxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Default values
	hitboxDamage = 0.0f;
	launchAmount = 0.0f;
	hitboxAngle = 0.0f;
	stunTime = 0.0f;

	damageMultiplier = 1.0f;
	launchMultiplier = 1.0f;

	shouldLaunchMatchCharacterDirection = false;
}

// Called when the game starts or when spawned
void AHitboxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHitboxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

