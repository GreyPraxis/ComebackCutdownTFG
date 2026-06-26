// Fill out your copyright notice in the Description page of Project Settings.


#include "EsperantoCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEsperantoCharacter::AEsperantoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;


	// Configure character movement
	//La rotación con animaciones o física?
	GetCharacterMovement()->bOrientRotationToMovement = false; // Face in the direction we are moving..

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 0.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	basePlayerHealth = 9999.0f;
	currentPlayerHealth = basePlayerHealth;

	//characterClass = ECharacterClass::VE_Fox;

	directionalInputLeftRight = EDirectionalInput::VE_Neutral;
	directionalInputUpDown = EDirectionalInput::VE_Default;

	characterState = ECharacterState::E_Idle;

	xStrong = 0.00f;
	yStrong = 0.00f;

	maxJumps = 2;
	jumpCount = 0;
	jumpSpeed = 1000.f;

	LastTapTime = 0.0f;
	LastTapDirection = EDirectionalInput::VE_Neutral;
	DoubleTapThreshold = 0.5f; // segundos
	bAxisPressed = false;
	LastReleaseTime = 0.0f;
	MinReleaseTime = 0.1f;

	//cantMove = ;
	//wasWeakAttackUsed = false;
	//wasNormalAttackUsed = false;
	//canAttack = true;

	spinCharged = 0;
}

//void AEsperantoCharacter::AttackWeak() {
//}

//void AEsperantoCharacter::AttackNormal() {
//}
