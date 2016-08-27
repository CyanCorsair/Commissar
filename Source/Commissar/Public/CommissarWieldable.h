// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommissarItem.h"
#include "GameFramework/Actor.h"
#include "CommissarWieldable.generated.h"

UENUM(BlueprintType)
enum class EWieldableRole : uint8
{
	SideArm,
	CloseAssault,
	Rifle,
	Heavy,
	Melee
};

UENUM(BlueprintType)
enum class EWieldableAttackType : uint8
{
	Beam,
	Explosive,
	SmallProjectile,
	Shard
};

UENUM(BlueprintType)
enum class EWieldableRarity : uint8
{
	Legacy,
	Military,
	Civilian,
	Exotic
};

USTRUCT(BlueprintType)
struct FWieldableAttachment
{
	GENERATED_BODY()

		UPROPERTY()
		FName FAttachmentSocket;

	UPROPERTY()
		TSubclassOf<class ACommissarWieldableComponent> Attachment;
};

UCLASS()
class COMMISSAR_API ACommissarWieldable : public ACommissarItem
{
	GENERATED_BODY()
	
	/** Properties */
	public:

		/** Location on gun mesh where projectiles should spawn. */
		UPROPERTY(EditDefaultsOnly, Category = Mesh)
			class USceneComponent* MuzzleLocation;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
			USkeletalMeshComponent* WieldableMesh;

		/** Gun muzzle's offset from the characters location */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
			FVector GunOffset;

		UPROPERTY(EditDefaultsOnly, Category = Mesh)
			TArray<FName> AttachmentSocketList;

		UPROPERTY(EditDefaultsOnly, Category = Mesh)
			TArray<struct FWieldableAttachment> Attachments;

		/** Projectile class to spawn */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
			class ACommissarAmmunition* AmmunitionClass;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
			class ACommissarAmmunition* CurrentlyHeldAmmo;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
			TArray<class ACommissarAmmunition*> AllowedAmmunitionClasses;

		/** Sound to play each time we fire */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
			class USoundBase* FireSound;

		/** AnimMontage to play each time we fire */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
			class UAnimMontage* FireAnimation;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics)
			int Durability;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics)
			float Stability;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistics)
			int Power;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property)
			EWieldableRole WeaponRole;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property)
			EWieldableAttackType AttackType;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property)
			EWieldableRarity WeaponRarity;

		bool bIsOnGround;
		bool bCanFire;

	private:

	protected:

	/** Methods */
	public:
		// Sets default values for this actor's properties
		ACommissarWieldable();

		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		// Called every frame
		virtual void Tick(float DeltaSeconds) override;

		// Interaction
		void OnEquipped();
		void OnUnEquipped();

		void WantsToFire();
		void OnBeginFire();
		void OnEndFire();

		UFUNCTION(BlueprintCallable, Category = Interaction)
			virtual void OnUsed() override;

		UFUNCTION(BlueprintCallable, Category = Interaction)
			virtual void PickedUp() override;

		UFUNCTION(BlueprintCallable, Category = Interaction)
			virtual void Dropped() override;

		int GetAmmunitionCount(class ACommissarCharacter* Owner, class ACommissarConsumableItem* AmmunitionClass);

		float CalculateStability(class ACommissarCharacter* Owner);

	private:

	protected:
	
};
