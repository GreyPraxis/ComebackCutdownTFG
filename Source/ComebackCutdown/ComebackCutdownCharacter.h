// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ComebackCutdownCharacter.generated.h"


UENUM(BlueprintType)
enum class EDirectionalInput : uint8
{
	VE_MovingRight	UMETA(DisplayName = "MOVING_RIGHT"),
	VE_Neutral		UMETA(DisplayName = "NOT_MOVING"),
	VE_MovingLeft	UMETA(DisplayName = "MOVING_LEFT"),

	VE_LookingUp	UMETA(DisplayName = "LOOKING_UP"),
	VE_Default		UMETA(DisplayName = "NOT_LOOKING"),
	VE_LookingDown	UMETA(DisplayName = "LOOKING_DOWN")
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	E_Idle			UMETA(DisplayName = "IDLE"),
	E_MovingRight			UMETA(DisplayName = "MOVING_RIGHT"),
	E_RunningRight		UMETA(DisplayName = "RUNNING_RIGHT"),
	E_MovingLeft			UMETA(DisplayName = "MOVING_LEFT"),
	E_RunningLeft		UMETA(DisplayName = "RUNNING_LEFT"),
	E_Crouching		UMETA(DisplayName = "CROUCH"),
	E_Jumping		UMETA(DisplayName = "JUMPING"),
	E_Block		UMETA(DisplayName = "BLOCKING"),
	E_Stunned		UMETA(DisplayName = "STUNNED"),
	//E_AirBlock		UMETA(DisplayName = "AIR_BLOCKING"),
	//E_CrouchBlock		UMETA(DisplayName = "CROUCH_BLOCKING"),
	E_Dead		UMETA(DisplayName = "DEAD")
};
/*
UENUM(BlueprintType)
enum class EInputType : uint8
{
	E_None			UMETA(DisplayName = "NONE"),
	E_Left			UMETA(DisplayName = "LEFT"),
	E_Right			UMETA(DisplayName = "RIGHT"),
	E_Up			UMETA(DisplayName = "UP"),
	E_Down			UMETA(DisplayName = "DOWN"),
	E_Jump			UMETA(DisplayName = "JUMP"),
	E_Block			UMETA(DisplayName = "BLOCK"),
	E_WeakAttack	UMETA(DisplayName = "WEAK_ATTACK"),
	E_NormalAttack	UMETA(DisplayName = "NORMAL_ATTACK"),
	E_SpecialAttack	UMETA(DisplayName = "SPECIAL_ATTACK"),
	E_StrongAttack	UMETA(DisplayName = "STRONG_ATTACK"),
	E_Throw			UMETA(DisplayName = "THROW"),
	E_ExAttack		UMETA(DisplayName = "SUPER_ATTACK")
};

UENUM(BlueprintType)
enum class EInputStatus : uint8
{
	E_None			UMETA(DisplayName = "NONE"),
	E_Press			UMETA(DisplayName = "PRESS"),
	E_Release			UMETA(DisplayName = "RELEASE"),
	E_Hold			UMETA(DisplayName = "HOLD")
};

USTRUCT(BlueprintType)
struct FInputInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		EInputType inputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		EInputStatus inputStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		int64 frame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		bool wasUsed;

};*/


UCLASS(config=Game)
class AComebackCutdownCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;


	UFUNCTION(BlueprintCallable)
		void AttackWeak(); //Add virtual??
	UFUNCTION(BlueprintCallable)
		void AttackNormal();
	UFUNCTION(BlueprintCallable)
		void AttackSpecial();
	UFUNCTION(BlueprintCallable)
		void AttackSpecialRelease();
	UFUNCTION(BlueprintCallable)
		void AttackStrong(); //Tilt+Boton instantaneo o Joystick derecho (configurable)
	void AttackStrongW();
	void AttackStrongN();
	UFUNCTION(BlueprintCallable)
		void AttackStrongRelease();
	void AttackStrongReleaseW();
	void AttackStrongReleaseN();

	// Keyboard-Only mode multiplayer
	//UFUNCTION(BlueprintCallable)
	//	void P2KeyboardAttackWeak();
	//UFUNCTION(BlueprintCallable)
	//	void P2KeyboardAttackNormal();
	//UFUNCTION(BlueprintCallable)
	//	void P2KeyboardAttackSpecial();
	//UFUNCTION(BlueprintCallable)
	//	void P2KeyboardAttackStrong();
	//UFUNCTION(BlueprintCallable)
	//	void P2KeyboardStartJump();
	//UFUNCTION(BlueprintCallable)
	//	void P2KeyboardStopJump();
	//UFUNCTION(BlueprintCallable)
	//	void P2KeyboardMoveRight(float value);
	//UFUNCTION(BlueprintCallable)
	//	void P2KeyboardUpDown(float value);
	//UFUNCTION(BlueprintCallable)
	//	void P2StartBlock();
	//UFUNCTION(BlueprintCallable)
	//	void P2StopBlock();


protected:
	// Aterriza tras caer en suelo
	virtual void Landed(const FHitResult& Hit) override;

	//Player used a weak attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
		bool canAttack;
	//Player used a weak attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
		bool wasWeakAttackUsed;
	//Player used a normal attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
		bool wasNormalAttackUsed;
	//Player used a special attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
		bool wasSpecialAttackUsed;
	//Player used a strong attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
		bool wasStrongAttackUsed;

	//Player used a special attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
		bool isCharging;

	bool weakHeld;
	bool normalHeld;

	// Maximum allowed time to charge a strong attack
	float maxInputHoldTime;

	// Timer handle used to track how long strong attacks were held
	FTimerHandle inputHeldTimer;


	//Saltos
	UFUNCTION(BlueprintCallable)
		void StartJump();
	UFUNCTION(BlueprintCallable)
		void StopJump();

	//Correr
	void Sprint();

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable)
		void MoveRight(float Val);
	UFUNCTION(BlueprintCallable)
		void UpDown(float Val);

	// Futura adiccion de movimiento en mando singleplayer
	void MoveRightController(float Val);
	void UpDownController(float Val);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float timeDirectionalXInputWasHeld;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		float timeDirectionalYInputWasHeld;

	//Antigua revisión de salto
	/** Handle touch inputs. */
	//void TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location);
	/** Handle touch stop event. */
	//void TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Blocking
	UFUNCTION(BlueprintCallable)
		void StartBlocking();

	// Blocking
	UFUNCTION(BlueprintCallable)
		void StopBlocking();

	// Blocking
	UFUNCTION(BlueprintCallable)
		void BreakBlock();

	// Damage the character
	UFUNCTION(BlueprintCallable)
		void TakeDamage(float _damageAmount, float _launch, float _angel, 
			FVector _hitLocation = FVector(0.0f, 0.0f, 0.0f), bool _shouldLaunchMatchCharacterDirection = false, 
			float stunTime = 0.0f,
			AActor* _instigator = nullptr);

	// Knockback the character in a direction
	UFUNCTION(BlueprintCallable)
		void HandleLaunch(float _launch, float _angel);

	// Move a character off of another hurtbox/collider smoothly
	UFUNCTION(BlueprintImplementableEvent)
		void MoveCharacterSmoothly(FVector _start, FVector _end);


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	//	ECharacterClass characterClass;

	//Amount of health of the current character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseHealth")
		float basePlayerHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CurrentHealth")
		float currentPlayerHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		int playerNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		AComebackCutdownCharacter* otherPlayer;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		EDirectionalInput directionalInputLeftRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool isFacingRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool isRunning;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crouch/LookUp")
		EDirectionalInput directionalInputUpDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WalkState")
		ECharacterState characterState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputStrongness")
		float xStrong;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InputStrongness")
		float yStrong;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		int jumpCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		int maxJumps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		float jumpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
		float LastTapTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
		EDirectionalInput LastTapDirection; // -1 izquierda, 1 derecha, 0 ninguna
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
		float DoubleTapThreshold; // segundos
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
		bool bAxisPressed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
		float LastReleaseTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
		float stopRunTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
		float MinReleaseTime;


	// for charging moves
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChargeMoves")
		bool chargingSpecial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChargeMoves")
		bool chargingStrong;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChargeMoves")
		float timeChargingSpecial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChargeMoves")
		float timeChargingStrong;

	// stunTimer from taking damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stun")
		float timeStunned;

	// stunTime from getting the shield hit
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
		float blockStun;
	// stunTime from getting the shield pierced
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Block")
		float blockShieldBreakStunFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		bool cantMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
		TArray<UStaticMeshComponent*> hurtboxes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
		bool isDeviceForMultiplePlayers;

	virtual void Tick(float DeltaTime) override;

public:
	AComebackCutdownCharacter();

	/** Returns SideViewCameraComponent subobject **/
	//FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	//FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
