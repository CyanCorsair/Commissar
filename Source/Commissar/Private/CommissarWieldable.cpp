// Fill out your copyright notice in the Description page of Project Settings.

#include "Commissar.h"
#include "CommissarWieldable.h"

#include "CommissarCharacter.h"
#include "CommissarItem.h"
#include "CommissarConsumableItem.h"
#include "CommissarProjectile.h"
#include "CommissarWieldableComponent.h"
#include "CommissarAmmunition.h"

#include "Engine.h"
#include "Animation/AnimInstance.h"

// Sets default values
ACommissarWieldable::ACommissarWieldable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set mesh
	WieldableMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	RootComponent = WieldableMesh;

	// Set the muzzle location
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(WieldableMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	bIsOnGround = true;
	bCanFire = false;
	bIsFiring = false;

	CurrentlyHeldAmmo = NULL;
}

// Called when the game starts or when spawned
void ACommissarWieldable::BeginPlay()
{
	Super::BeginPlay();

	WieldableMesh->SetVisibility(false);
	
}

// Called every frame
void ACommissarWieldable::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ACommissarWieldable::OnUsed()
{
	WantsToFire();
}

void ACommissarWieldable::PickedUp()
{
	if (Mesh)
	{
		// Hide the world mesh, show the Wieldable Mesh
		Mesh->SetVisibility(false);
		WieldableMesh->SetVisibility(true);
		bIsOnGround = false;
	}
}

void ACommissarWieldable::Dropped()
{
	ACommissarCharacter* Owner = Cast<ACommissarCharacter>(GetOwner());
	FRotator SpawnRotation(0.f, 0.f, 0.f);
	FVector SpawnLocation(0.f, 0.f, 0.f);

	if (Owner)
	{
		TArray<ACommissarItem*> Inventory = Owner->GetInventory();
		Inventory.Remove(this);

		SpawnRotation = Owner->GetControlRotation();
		SpawnLocation = Owner->GetActorLocation();
		SpawnLocation.X = (SpawnLocation.X + Owner->MaxUseDistance);
	}

	Mesh->SetVisibility(true);
	WieldableMesh->SetVisibility(false);
	bIsOnGround = true;
}

// Firing logic

void ACommissarWieldable::WantsToFire()
{
	// Check currently held ammunition
	// If none chambered, force owner to reload
	// If none in owner inventory, play NoAmmo sound
	// If successful, invoke OnBeginFire

	ACommissarCharacter* Owner = Cast<ACommissarCharacter>(GetOwner());

	if (CurrentlyHeldAmmo != NULL)
	{
		if (CurrentlyHeldAmmo->Uses == 0)
		{
			// If we have more than 0 of an ammo item, simply force a reload
			// Otherwise, destroy the actor and set var explicitly to null
			if (CurrentlyHeldAmmo->StackSize > 0)
			{
				bCanFire = false;
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation()); return;
			}
			else
			{
				bCanFire = false;
				CurrentlyHeldAmmo->Destroy();
				CurrentlyHeldAmmo = NULL;
			}
			
		}
		else
		{
			OnBeginFire(Owner);
			bCanFire = true;
		}
	}
	else
	{
		// If reload fires but no ammo exists, switch off fire
		if (CurrentlyHeldAmmo == NULL) bCanFire = false;
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation()); return;
	}
}

void ACommissarWieldable::OnBeginFire(ACommissarCharacter* Owner)
{

	// Fire a projectile
	if (bCanFire)
	{
		bIsFiring = true;

		// try and fire a projectile
		const FRotator SpawnRotation = Owner->GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			World->SpawnActor<ACommissarProjectile>(SpawnLocation, SpawnRotation);
			CurrentlyHeldAmmo->ReduceUses();

			// try and play the sound if specified
			if (FireSound != NULL)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			// try and play a firing animation if specified
			if (FireAnimation != NULL)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Owner->Mesh1P->GetAnimInstance();
				if (AnimInstance != NULL)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}
		}
	}
}

void ACommissarWieldable::OnEndFire()
{
	bIsFiring = false;
}

// Equipping logic
void ACommissarWieldable::OnEquipped()
{

}

void ACommissarWieldable::OnUnEquipped()
{

}