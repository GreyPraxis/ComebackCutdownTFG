// Copyright Epic Games, Inc. All Rights Reserved.

#include "ComebackCutdownCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ComebackCutdownGameMode.h"
//Check 

AComebackCutdownCharacter::AComebackCutdownCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	//La rotación con animaciones o física?
	GetCharacterMovement()->bOrientRotationToMovement = false; // Face in the direction we are moving..

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	basePlayerHealth = 9999.0f;
	currentPlayerHealth = basePlayerHealth;

	//characterClass = ECharacterClass::VE_Default;
	playerNumber = 0;
	directionalInputLeftRight = EDirectionalInput::VE_Neutral;
	directionalInputUpDown = EDirectionalInput::VE_Default;
	isFacingRight = false;
	isRunning = false;

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
	stopRunTime = 0.0f;

	timeStunned = 0.0f;

	blockShieldBreakStunFrames = 150.0f;

	cantMove = false;
	wasWeakAttackUsed = false;
	wasNormalAttackUsed = false;
	canAttack = true;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AComebackCutdownCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (isDeviceForMultiplePlayers) {
		if (auto gameMode = Cast<AComebackCutdownGameMode>(GetWorld()->GetAuthGameMode())) {

			if (gameMode->player1 == this) {
				PlayerInputComponent->BindAxis("MoveRight", this, &AComebackCutdownCharacter::MoveRight);
				PlayerInputComponent->BindAxis("UpDown", this, &AComebackCutdownCharacter::UpDown);
			}
			/*else //Player 2 inputs
			{
				PlayerInputComponent->BindAxis("MoveRightP2", this, &AComebackCutdownCharacter::MoveRight);
				PlayerInputComponent->BindAxis("UpDownP2", this, &AComebackCutdownCharacter::UpDown);
			}*/

			PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AComebackCutdownCharacter::StartJump);
			PlayerInputComponent->BindAction("Jump", IE_Released, this, &AComebackCutdownCharacter::StopJump);

			//Añadir funcionalidad
			PlayerInputComponent->BindAxis("MoveRightController", this, &AComebackCutdownCharacter::MoveRightController);
			PlayerInputComponent->BindAxis("UpDownController", this, &AComebackCutdownCharacter::UpDownController);

			PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AComebackCutdownCharacter::StartBlocking);
			PlayerInputComponent->BindAction("Block", IE_Released, this, &AComebackCutdownCharacter::StopBlocking);

			// Movimientos
			PlayerInputComponent->BindAction("AttackWeak", IE_Pressed, this, &AComebackCutdownCharacter::AttackWeak);
			PlayerInputComponent->BindAction("AttackNormal", IE_Pressed, this, &AComebackCutdownCharacter::AttackNormal);
			PlayerInputComponent->BindAction("AttackSpecial", IE_Pressed, this, &AComebackCutdownCharacter::AttackSpecial);
		}
	}
	// set up gameplay key bindings
	else {

		PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AComebackCutdownCharacter::StartJump);
		PlayerInputComponent->BindAction("Jump", IE_Released, this, &AComebackCutdownCharacter::StopJump);
		PlayerInputComponent->BindAxis("MoveRight", this, &AComebackCutdownCharacter::MoveRight);
		PlayerInputComponent->BindAxis("UpDown", this, &AComebackCutdownCharacter::UpDown);

	// Añadir funcionalidad
		PlayerInputComponent->BindAxis("MoveRightController", this, &AComebackCutdownCharacter::MoveRightController);
		PlayerInputComponent->BindAxis("UpDownController", this, &AComebackCutdownCharacter::UpDownController);


		PlayerInputComponent->BindAction("Block", IE_Pressed, this, &AComebackCutdownCharacter::StartBlocking);
		PlayerInputComponent->BindAction("Block", IE_Released, this, &AComebackCutdownCharacter::StopBlocking);

		// Movimientos
		PlayerInputComponent->BindAction("AttackWeak", IE_Pressed, this, &AComebackCutdownCharacter::AttackWeak);
		PlayerInputComponent->BindAction("AttackNormal", IE_Pressed, this, &AComebackCutdownCharacter::AttackNormal);
		PlayerInputComponent->BindAction("AttackSpecial", IE_Pressed, this, &AComebackCutdownCharacter::AttackSpecial);


	}

	// Antiguo codigo de salto
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &AComebackCutdownCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &AComebackCutdownCharacter::TouchStopped);
}

void AComebackCutdownCharacter::Sprint() {

}

void AComebackCutdownCharacter::MoveRight(float Value)
{
	if (isDeviceForMultiplePlayers) {

		//UE_LOG(LogTemp, Warning, TEXT("Movement value %f."), Value);
		float CurrentTime = GetWorld()->GetTimeSeconds();
		//Control de dirección
		if (Value > 0.20f) {//Provisional, revisar
			if ((LastTapDirection == EDirectionalInput::VE_MovingRight &&
				CurrentTime - LastTapTime <= DoubleTapThreshold && !bAxisPressed &&
				CurrentTime - LastReleaseTime <= MinReleaseTime) || isRunning) {
				//Comieza sprint
				UE_LOG(LogTemp, Warning, TEXT("CorreDer"));
				isRunning = true;
				stopRunTime = 0.1f;
			}
			directionalInputLeftRight = EDirectionalInput::VE_MovingRight;
			LastTapDirection = directionalInputLeftRight;

			LastTapTime = CurrentTime;
			bAxisPressed = true;
		}
		else if (Value < -0.20f) {//Provisional, revisar
			if ((LastTapDirection == EDirectionalInput::VE_MovingLeft &&
				CurrentTime - LastTapTime <= DoubleTapThreshold && !bAxisPressed &&
				CurrentTime - LastReleaseTime <= MinReleaseTime) || isRunning) {
				//Comieza sprint
				UE_LOG(LogTemp, Warning, TEXT("CorreIzq"));
				isRunning = true;
				stopRunTime = 0.1f;
			}
			directionalInputLeftRight = EDirectionalInput::VE_MovingLeft;
			LastTapDirection = directionalInputLeftRight;

			LastTapTime = CurrentTime;
			bAxisPressed = true;
		}
		else {//Provisional, revisar
			directionalInputLeftRight = EDirectionalInput::VE_Neutral;
			if (bAxisPressed) {
				LastReleaseTime = CurrentTime;
			}
			//isRunning = false;
			bAxisPressed = false;
		}
		xStrong = Value;
		//Control de Walk/Sprint

		if (stopRunTime <= 0.0f && isRunning) {
			isRunning = false;
			UE_LOG(LogTemp, Warning, TEXT("ParaCorrer"));
		}
		// add movement in that direction
		if (!cantMove && characterState != ECharacterState::E_Block && characterState != ECharacterState::E_Stunned) {
			if (!GetCharacterMovement()->IsFalling()) {

				if (Value > 0.20f) {
					SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
					isFacingRight = true;
				}
				else if (Value < -0.20f) {
					SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
					isFacingRight = false;

				}
			}
			if (isRunning) {
				AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
			}
			else {
				AddMovementInput(FVector(0.f, -0.8f, 0.f), Value);

			}

		}
	}
}

void AComebackCutdownCharacter::MoveRightController(float Value)
{
	if (!isDeviceForMultiplePlayers) {

		//UE_LOG(LogTemp, Warning, TEXT("Movement value %f."), Value);
		float CurrentTime = GetWorld()->GetTimeSeconds();
		//Control de dirección
		if (Value > 0.20f) {//Provisional, revisar
			if ((LastTapDirection == EDirectionalInput::VE_MovingRight &&
				CurrentTime - LastTapTime <= DoubleTapThreshold && !bAxisPressed &&
				CurrentTime - LastReleaseTime <= MinReleaseTime) || isRunning) {
				//Comieza sprint
				UE_LOG(LogTemp, Warning, TEXT("CorreDer"));
				isRunning = true;
				stopRunTime = 0.1f;
			}
			directionalInputLeftRight = EDirectionalInput::VE_MovingRight;
			LastTapDirection = directionalInputLeftRight;

			LastTapTime = CurrentTime;
			bAxisPressed = true;
		}
		else if (Value < -0.20f) {//Provisional, revisar
			if ((LastTapDirection == EDirectionalInput::VE_MovingLeft &&
				CurrentTime - LastTapTime <= DoubleTapThreshold && !bAxisPressed &&
				CurrentTime - LastReleaseTime <= MinReleaseTime) || isRunning) {
				//Comieza sprint
				UE_LOG(LogTemp, Warning, TEXT("CorreIzq"));
				isRunning = true;
				stopRunTime = 0.1f;
			}
			directionalInputLeftRight = EDirectionalInput::VE_MovingLeft;
			LastTapDirection = directionalInputLeftRight;

			LastTapTime = CurrentTime;
			bAxisPressed = true;
		}
		else {//Provisional, revisar
			directionalInputLeftRight = EDirectionalInput::VE_Neutral;
			if (bAxisPressed) {
				LastReleaseTime = CurrentTime;
			}
			//isRunning = false;
			bAxisPressed = false;
		}
		xStrong = Value;
		//Control de Walk/Sprint

		if (stopRunTime <= 0.0f && isRunning) {
			isRunning = false;
			UE_LOG(LogTemp, Warning, TEXT("ParaCorrer"));
		}
		// add movement in that direction
		if (!cantMove && characterState != ECharacterState::E_Block && characterState != ECharacterState::E_Stunned) {
			if (!GetCharacterMovement()->IsFalling()) {

				if (Value > 0.20f) {
					SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
					isFacingRight = true;
				}
				else if (Value < -0.20f) {
					SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
					isFacingRight = false;

				}
			}
			if (isRunning) {
				AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
			}
			else {
				AddMovementInput(FVector(0.f, -0.8f, 0.f), Value);

			}

		}
	}
}

void AComebackCutdownCharacter::UpDown(float Value)
{
	if (isDeviceForMultiplePlayers) {
		if (Value > 0.20f) {//Provisional, revisar
			directionalInputUpDown = EDirectionalInput::VE_LookingUp;
		}
		else if (Value < -0.20f) {//Provisional, revisar
			directionalInputUpDown = EDirectionalInput::VE_Crouching;
			//Crouch
		}
		else {//Provisional, revisar
			directionalInputUpDown = EDirectionalInput::VE_Default;
		}
		yStrong = Value;
	}
}

void AComebackCutdownCharacter::UpDownController(float Value)
{
	if (!isDeviceForMultiplePlayers) {
		if (Value > 0.20f) {//Provisional, revisar
			directionalInputUpDown = EDirectionalInput::VE_LookingUp;
		}
		else if (Value < -0.20f) {//Provisional, revisar
			directionalInputUpDown = EDirectionalInput::VE_Crouching;
			//Crouch
		}
		else {//Provisional, revisar
			directionalInputUpDown = EDirectionalInput::VE_Default;
		}
		yStrong = Value;
	}
}

void AComebackCutdownCharacter::Landed(const FHitResult& Hit) {
	ACharacter::Landed(Hit);

	// Resetear numero de saltos
	jumpCount = 0;
}

void AComebackCutdownCharacter::StartJump()
{
	// jump on any touch
	//Jump();
	//if (jumpCount < maxJumps && !cantMove) {
		// Lanzar hacia arriba el personaje
	//	LaunchCharacter(FVector(0.0f, 0.0f, jumpSpeed), false, true);
		// Contar salto
	//	++jumpCount;
	//}
	if (!cantMove && characterState != ECharacterState::E_Dead 
		&& characterState != ECharacterState::E_Stunned) {
		ACharacter::Jump();
	}
	isRunning = false;
}

void AComebackCutdownCharacter::StopJump()
{
	if (!cantMove && characterState != ECharacterState::E_Dead
		&& characterState != ECharacterState::E_Stunned
		&& characterState != ECharacterState::E_Block) {
		StopJumping();
	}
}

void AComebackCutdownCharacter::AttackWeak() {

	UE_LOG(LogTemp, Warning, TEXT("Ataque debil"));
	//Revisar direccionalidad y aire/suelo
	if (canAttack == true && !cantMove && characterState != ECharacterState::E_Dead
		&& characterState != ECharacterState::E_Stunned
		&& characterState != ECharacterState::E_Block) {
		wasWeakAttackUsed = true;
		canAttack = false;
		cantMove = true;
	}


	if (GetCharacterMovement()->IsMovingOnGround()) { //En suelo
		if (directionalInputLeftRight == EDirectionalInput::VE_Neutral && directionalInputUpDown == EDirectionalInput::VE_Default) {
			UE_LOG(LogTemp, Warning, TEXT("Jab"));
		}
		else if (abs(xStrong) >= abs(yStrong)) {
			if (xStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("Sidetilt-Right"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("Sidetilt-Left"));
		}
		else {
			if (yStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("uptilt"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("downtilt"));
		}
	}

	else {	//En aire
		if (directionalInputLeftRight == EDirectionalInput::VE_Neutral && directionalInputUpDown == EDirectionalInput::VE_Default) {
			UE_LOG(LogTemp, Warning, TEXT("Nair"));
		}
		else if (abs(xStrong) >= abs(yStrong)) {
			if (xStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("Right-Air"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("Left-Air"));
		}
		else {
			if (yStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("upair"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("downair"));
		}
	}

}

void AComebackCutdownCharacter::AttackNormal() {
	UE_LOG(LogTemp, Warning, TEXT("Ataque normal"));

	//Revisar direccionalidad y aire/suelo
	if (canAttack == true && !cantMove && characterState != ECharacterState::E_Dead
		&& characterState != ECharacterState::E_Stunned
		&& characterState != ECharacterState::E_Block) {
		wasNormalAttackUsed = true;
		canAttack = false;
		cantMove = true;
	}


	if (GetCharacterMovement()->IsMovingOnGround()) { //En suelo
		if (directionalInputLeftRight == EDirectionalInput::VE_Neutral && directionalInputUpDown == EDirectionalInput::VE_Default) {
			UE_LOG(LogTemp, Warning, TEXT("Jab"));
		}
		else if (abs(xStrong) >= abs(yStrong)) {
			if (xStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("Sidetilt-Right"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("Sidetilt-Left"));
		}
		else {
			if (yStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("uptilt"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("downtilt"));
		}
	}

	else {	//En aire
		if (directionalInputLeftRight == EDirectionalInput::VE_Neutral && directionalInputUpDown == EDirectionalInput::VE_Default) {
			UE_LOG(LogTemp, Warning, TEXT("Nair"));
		}
		else if (abs(xStrong) >= abs(yStrong)) {
			if (xStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("Right-Air"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("Left-Air"));
		}
		else {
			if (yStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("upair"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("downair"));
		}
	}


}

void AComebackCutdownCharacter::AttackSpecial() {
	UE_LOG(LogTemp, Warning, TEXT("Ataque especial"));

	//Revisar direccionalidad y aire/suelo


	if (GetCharacterMovement()->IsMovingOnGround()) { //En suelo
		if (directionalInputLeftRight == EDirectionalInput::VE_Neutral && directionalInputUpDown == EDirectionalInput::VE_Default) {
			UE_LOG(LogTemp, Warning, TEXT("Jab"));
		}
		else if (abs(xStrong) >= abs(yStrong)) {
			if (xStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("Sidetilt-Right"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("Sidetilt-Left"));
		}
		else {
			if (yStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("uptilt"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("downtilt"));
		}
	}

	else {	//En aire
		if (directionalInputLeftRight == EDirectionalInput::VE_Neutral && directionalInputUpDown == EDirectionalInput::VE_Default) {
			UE_LOG(LogTemp, Warning, TEXT("Nair"));
		}
		else if (abs(xStrong) >= abs(yStrong)) {
			if (xStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("Right-Air"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("Left-Air"));
		}
		else {
			if (yStrong > 0.00f) {
				UE_LOG(LogTemp, Warning, TEXT("upair"));
			}
			else UE_LOG(LogTemp, Warning, TEXT("downair"));
		}
	}

}
/*
void AComebackCutdownCharacter::AttackStrong() {
	UE_LOG(LogTemp, Warning, TEXT("Ataque fuerte"));

	if (abs(xStrong) >= abs(yStrong)) {
		if (xStrong > 0.00f) {
			UE_LOG(LogTemp, Warning, TEXT("Sidestrong-Right"));
		}
		else UE_LOG(LogTemp, Warning, TEXT("Sidestrong-Left"));
	}
	else {
		if (yStrong > 0.00f) {
			UE_LOG(LogTemp, Warning, TEXT("upstrong"));
		}
		else UE_LOG(LogTemp, Warning, TEXT("downstrong"));
	}
}
*/ 

void AComebackCutdownCharacter::StartBlocking() {
	if (characterState != ECharacterState::E_Dead
		&& characterState != ECharacterState::E_Stunned) {
		characterState = ECharacterState::E_Block;
	}
}

void AComebackCutdownCharacter::StopBlocking() {
	if (characterState == ECharacterState::E_Block) {
		characterState = ECharacterState::E_Idle;
	}
}


void AComebackCutdownCharacter::TakeDamage(float _damageAmount, 
	float _launch, float _angel,
	FVector _hitLocation, bool _shouldLaunchMatchCharacterDirection, 
	float stuntime,	AActor* _instigator) {
	//UE_LOG(LogTemp, Warning, TEXT("Taking damage for %f points."), _damageAmount);
	
	if (characterState != ECharacterState::E_Block) {
		currentPlayerHealth -= _damageAmount;

		if (currentPlayerHealth < 0.0f) currentPlayerHealth = 0.0f;

		timeStunned = stuntime;
		if (timeStunned > 0) {
			characterState = ECharacterState::E_Stunned;
		}

		if (_shouldLaunchMatchCharacterDirection)
		{
			// If there is an owner/creator of the hitbox
			if (_instigator)
			{
				//If the owner/creator is a playable character
				if (AComebackCutdownCharacter* owningCharacter = Cast<AComebackCutdownCharacter>(_instigator))
				{
					if (owningCharacter->isFacingRight)
					{
						HandleLaunch(_launch, acosf((-cosf(_angel)))); // invert x-axis launch angle
					}
					else
					{
						HandleLaunch(_launch, _angel);
					}
				}
			}
			else
			{
				if (!isFacingRight)
				{
					HandleLaunch(_launch, _angel);
				}
				else
				{
					HandleLaunch(_launch, acosf((-cosf(_angel)))); // invert x-axis launch angle
				}
			}
		}
		else
		{
			if (GetActorLocation().Y >= _hitLocation.Y)
			{
				HandleLaunch(_launch, _angel);
			}
			else
			{
				HandleLaunch(_launch, acosf((-cosf(_angel)))); // invert x-axis launch angle
			}
		}

	}
	else { //If blocking, handle launch only in X axis
		//(cosf(_angel)*_launch/2) = Launch distance in X / 2
		if (_shouldLaunchMatchCharacterDirection)
		{
			// If there is an owner/creator of the hitbox
			if (_instigator)
			{
				//If the owner/creator is a playable character
				if (AComebackCutdownCharacter* owningCharacter = Cast<AComebackCutdownCharacter>(_instigator))
				{
					if (owningCharacter->isFacingRight)
					{
						HandleLaunch(cosf(_angel) * _launch / 2, acos(-1.0));// invert x-axis launch angle
					}
					else
					{
						HandleLaunch(cosf(_angel) * _launch / 3, 0);
					}
				}
			}
			else
			{
				if (!isFacingRight)
				{
					HandleLaunch(cosf(_angel) * _launch / 2, acos(-1.0));
				}
				else
				{
					HandleLaunch(cosf(_angel) * _launch / 2, 0); // invert x-axis launch angle
				}
			}
		}
		else
		{
			if (GetActorLocation().Y >= _hitLocation.Y)
			{
				HandleLaunch(cosf(_angel) * _launch / 2, acos(-1.0));
			}
			else
			{
				HandleLaunch(cosf(_angel) * _launch / 2, 0); // invert x-axis launch angle
			}
		}

	}
}

void AComebackCutdownCharacter::HandleLaunch(float _launch, float angel) {
	// fabs es solo el valor absolute del float

	//UE_LOG(LogTemp, Warning, TEXT("Being pushed for x(%f)."), cosf(angel) * _launch);
	//UE_LOG(LogTemp, Warning, TEXT("Cos del angulo (%f)."), cosf(angel));
	//UE_LOG(LogTemp, Warning, TEXT("Being pushed for y(%f)."), sinf(angel) * _launch);
	//UE_LOG(LogTemp, Warning, TEXT("Seno del angulo (%f)."), sinf(angel) * _launch);
	LaunchCharacter(FVector(0.0f, cosf(angel) * _launch, sinf(angel) * _launch), false, false);
}

void AComebackCutdownCharacter::BreakBlock() {
	characterState = ECharacterState::E_Stunned;
	timeStunned = blockShieldBreakStunFrames;
}

void AComebackCutdownCharacter::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	// Si lo hacemos funcionar con ints en vez de deltaTime
	// obtendremos un sistema con frames exactos.
	if (stopRunTime > 0) {
		stopRunTime -= DeltaTime;
	}
	if (timeStunned > 0) {
		timeStunned -= DeltaTime;
		if (timeStunned <= 0) {
			characterState = ECharacterState::E_Idle;
		}
	}

}




// P2 Stuff
/*
void AComebackCutdownCharacter::P2KeyboardAttackWeak() {
	AttackWeak();
}
void AComebackCutdownCharacter::P2KeyboardAttackNormal() {
	AttackNormal();

}
void AComebackCutdownCharacter::P2KeyboardAttackSpecial() {
	AttackSpecial();

}
void AComebackCutdownCharacter::P2KeyboardAttackStrong() {

}
void AComebackCutdownCharacter::P2KeyboardStartJump() {
	StartJump();
}
void AComebackCutdownCharacter::P2KeyboardStopJump() {
	StopJump();
}
void AComebackCutdownCharacter::P2KeyboardMoveRight(float value) {

	//UE_LOG(LogTemp, Warning, TEXT("Movement value %f."), value);
	MoveRight(value);
}
void AComebackCutdownCharacter::P2KeyboardUpDown(float value) {
	UpDown(value);
}
void AComebackCutdownCharacter::P2StartBlock() {
	StartBlocking();
}
void AComebackCutdownCharacter::P2StopBlock() {
	StopBlocking();
}*/