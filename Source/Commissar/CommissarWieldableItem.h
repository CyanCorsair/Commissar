// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommissarItem.h"
#include "CommissarWieldableItem.generated.h"
/**
 * 
 */
UCLASS()
class COMMISSAR_API ACommissarWieldableItem : public ACommissarItem
{
	GENERATED_BODY()
	
	/** Properties */
	public:

		/** Gun mesh: 1st person view (seen only by self) */
		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
			class USkeletalMeshComponent* FP_Gun;

		/** Location on gun mesh where projectiles should spawn. */
		UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
			class USceneComponent* FP_MuzzleLocation;

		/** Gun muzzle's offset from the characters location */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
			FVector GunOffset;

		/** Projectile class to spawn */
		UPROPERTY(EditDefaultsOnly, Category = Projectile)
			TSubclassOf<class ACommissarProjectile> ProjectileClass;

		/** Sound to play each time we fire */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
			class USoundBase* FireSound;

		/** AnimMontage to play each time we fire */
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
			class UAnimMontage* FireAnimation;

	private:

	protected:

	/** Methods */
	public:
		ACommissarWieldableItem();

		void OnUsed();

	private:

	protected:
};
