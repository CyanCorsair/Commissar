// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "CommissarCharacter.generated.h"

class UInputComponent;

UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	Stopped,
	Walking,
	Running,
	Sprinting
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Moving,
	Idle,
	Talking,
	UsingCharacterMenu,
	UsingPauseMenu,
	ZeroG,
	Dead
};

UCLASS(config=Game)
class ACommissarCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Inventory setup */
	UPROPERTY(EditAnywhere, Category = Inventory)
		TArray<class ACommissarItem*> ItemInventory;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;

	TArray<class ACommissarItem*, TFixedAllocator<10>> QuickInventory;

	int InventoryGridSquareSize;
	int InventoryGridX;
	int InventoryGridY;

	/** Character base attributes */
	float DefaultMaxWalkingSpeed = 0.0f;

	UPROPERTY(EditAnywhere, Category = CharacterAttributes)
	int MaxHealth;

	UPROPERTY(EditAnywhere, Category = CharacterAttributes)
	int Health;

	UPROPERTY(EditAnywhere, Category = CharacterAttributes)
	int MaxShields;

	UPROPERTY(EditAnywhere, Category = CharacterAttributes)
	int Shields;

	UPROPERTY(EditAnywhere, Category = Inventory)
	int MaxCredits;

	UPROPERTY(EditAnywhere, Category = Inventory)
	int Credits;

	UPROPERTY(EditAnywhere, Category = Inventory)
	int MaxMatter;

	UPROPERTY(EditAnywhere, Category = Inventory)
	int Matter;

	UPROPERTY(EditAnywhere, Category = Inventory)
	class ACommissarWieldableItem* CurrentlyHeld;

	UPROPERTY(EditAnywhere, Category = Inventory)
	class ACommissarWearableItem* CurrentlyWorn;

	/** Weapon modifiers */
	UPROPERTY(EditAnywhere, Category = CharacterAttributes)
	float MaxWeaponSpread;

	UPROPERTY(EditAnywhere, Category = CharacterAttributes)
	float WeaponSpread;

	UPROPERTY(EditAnywhere, Category = CharacterAttributes)
	float BaseWeaponSpreadReduction;

	UPROPERTY(EditAnywhere, Category = CharacterAttributes)
	float BaseWeaponReloadSpeed;

	UPROPERTY(EditAnywhere, Category = CharacterAttributes)
	float BaseMeleeHitSpeed;

	UPROPERTY(EditAnywhere, Category = CharacterAttributes)
	float BaseMeleeHitStrength;

	/** Do we want to trace a line? */
	bool bDrawDebugTrace;

	void KillCharacter();

public:
	ACommissarCharacter();

	virtual void BeginPlay();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Mesh1P;

	/** Character state monitors */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	ECharacterState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	ECharacterMovementState CurrentMovementState;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float MaxUseDistance;

	ACommissarItem* GetUsableInView();
	void PickUpItem(ACommissarItem* item);

	class ACommissarWieldableItem* CurrentlyHeldItem;

	void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Inventory", CompactNodeTitle = "GetInv", Keywords = "Get Player Inventory"), Category = Inventory)
		TArray<class ACommissarItem*> GetInventory();

	/** Skill & attribute blueprint declarations */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	float DisplayHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	float DisplayShields;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	float DisplayMatter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	float DisplayCredits;

	/** NPC Only attributes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	float Leadership;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	int Initiative;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	int Morale;

	/** Attribute and skill modifier methods */

	/** Character owned skils */
	UPROPERTY(EditAnywhere, Category = CharacterSkills)
		TArray<class UCommissarBaseSkill*> SkillList;

	UFUNCTION(BlueprintCallable, Category = "Attribute Accessors")
	int GetAttributeValue(FString AttributeName);

	UFUNCTION(BlueprintCallable, Category = "Attribute Accessors")
	void SetAttributeValue(FString AttributeName, int Amount);

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Uses an item in view. */
	void OnUse();

	/** Handled switching sprinting on/off */
	void Sprint();
	void StopSprinting();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/** Crouching toggle. */
	void StartCrouch();
	void EndCrouch();

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void ToggleInventory();

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

