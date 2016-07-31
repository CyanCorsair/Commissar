// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "CommissarCharacter.generated.h"

class UInputComponent;

/** Base character skill struct */
USTRUCT()
struct FCharacterSkill
{
	GENERATED_BODY()

	/** Skill UI display name and shorthand */
	FString FriendlyName;
	FString SystemName;

	/** Skill description strings */
	FString UntrainedDescription;
	FString TrainedDescription;
	FString AdvancedDescription;
	FString MasterDescription;

	/** Experience cost integers */
	int BaseExperienceCost;
	int ExperienceCost;
	int ExperienceCostModifier;

	/** Current level of skill */
	int CurrentLevel;

	/** Icon sprites */
	UTexture2D* IconLarge;
	UTexture2D* IconSmall;

	/** 
	* Does the owner have access? 
	* Used for Advanced Training
	*/
	bool bHasAccess;
};

UENUM()
enum class CharacterMovementState
{
	Stopped,
	Walking,
	Running,
	Sprinting
};

UENUM()
enum class CharacterState
{
	Moving,
	Idle,
	Talking,
	UsingMenu,
	Dead
};

UCLASS(config=Game)
class ACommissarCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Character state monitors */
	CharacterState CurrentState;
	CharacterMovementState CurrentMovementState;

	/** Inventory setup */
	UPROPERTY(EditAnywhere, Category = Inventory)
		TArray<class ACommissarItem*> ItemInventory;

	bool bIsInventoryOpen;
	int InventoryGridSquareSize;
	int InventoryGridX;
	int InventoryGridY;

	/** Character base attributes */
	float DefaultMaxWalkingSpeed = 0.0f;

	int MaxHealth;
	int Health;

	int MaxShields;
	int Shields;

	int MaxCredits;
	int Credits;

	int MaxMatter;
	int Matter;

	class ACommissarWieldableItem* CurrentlyHeld;
	class ACommissarWearableItem* CurrentlyWorn;

	/** Weapon modifiers */
	float MaxWeaponSpread;
	float WeaponSpread;
	float BaseWeaponSpreadReduction;
	float BaseWeaponReloadSpeed;

	float BaseMeleeHitSpeed;
	float BaseMeleeHitStrength;

	/** Player only skill declarations */
	FCharacterSkill ElectronicSecurity;
	FCharacterSkill MechanicalSecurity;
	FCharacterSkill Scavenging;
	FCharacterSkill Dialogue;

	/** Advanced Training skill declarations */
	FCharacterSkill RigEngineering;
	FCharacterSkill WeaponEngineering;

	/** Do we want to trace a line? */
	bool bDrawDebugTrace;

public:
	ACommissarCharacter();

	virtual void BeginPlay();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ACommissarProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

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

	/** NPC Only attributes */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	float Leadership;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	int Initiative;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	int Morale;

	/** Skill declarations - NPC & Player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	FCharacterSkill WeaponSkill;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	FCharacterSkill BallisticSkill_Sidearm;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	FCharacterSkill BallisticSkill_CloseAssault;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	FCharacterSkill BallisticSkill_Heavy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	FCharacterSkill BallisticSkill_Rifles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	FCharacterSkill Medicine;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CharacterProperties)
	FCharacterSkill FieldRepair;

	/** Attribute and skill modifier methods */

	int GetHealth();
	int GetShields();
	int GetMatter();
	int GetCredits();
	
	void SetHealth(int HealthVariable, int Amount);
	void SetShields(int ShieldVariable, int Amount);
	void SetMatter(int MatterVariable, int Amount);
	void SetCredits(int Amount);

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

