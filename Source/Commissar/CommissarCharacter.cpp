// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Commissar.h"
#include "CommissarCharacter.h"
#include "CommissarItem.h"
#include "CommissarWearableItem.h"
#include "CommissarWieldableItem.h"
#include "CommissarConsumableItem.h"
#include "CommissarProjectile.h"

#include "CommissarBaseSkill.h"
#include "CommissarMedicineSkill.h"

#include "Engine.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ACommissarCharacter

ACommissarCharacter::ACommissarCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Allow the character to crouch
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Inventory setup
	bIsInventoryOpen = false;
	InventoryGridSquareSize = 64;
	InventoryGridX, InventoryGridY = 16;
	MaxUseDistance = 100.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 30.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	// Character attribute setup
	MaxHealth = 100, Health = 100;
	MaxCredits = 1000000, Credits = 250;
	MaxMatter = 2480, Matter = 100;
	MaxShields = 0, Shields = 0;

	DisplayHealth = Health;
	DisplayShields = Shields;
	DisplayCredits = Credits;
	DisplayMatter = Matter;

	CurrentlyWorn = NULL;
	CurrentlyHeld = NULL;

	if (CurrentlyWorn != NULL)
	{
		MaxShields = CurrentlyWorn->MaxShieldCount;
		Shields = MaxShields;
	}

	// Skill definitions
	UCommissarMedicineSkill* Medicine = NewObject<UCommissarMedicineSkill>();

	SkillList.Add(Medicine);
}


void ACommissarCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	DefaultMaxWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); //Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
}


//////////////////////////////////////////////////////////////////////////
// Input

void ACommissarCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ACommissarCharacter::ToggleInventory);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ACommissarCharacter::Sprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ACommissarCharacter::StopSprinting);

	InputComponent->BindAction("Crouch", IE_Pressed, this, &ACommissarCharacter::StartCrouch);
	InputComponent->BindAction("Crouch", IE_Released, this, &ACommissarCharacter::EndCrouch);

	InputComponent->BindAction("Use", IE_Pressed, this, &ACommissarCharacter::OnUse);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ACommissarCharacter::OnFire);

	InputComponent->BindAxis("MoveForward", this, &ACommissarCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACommissarCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &ACommissarCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &ACommissarCharacter::LookUpAtRate);
}


void ACommissarCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			// spawn the projectile at the muzzle
			World->SpawnActor<ACommissarProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}

}


void ACommissarCharacter::OnUse() {
	ACommissarItem* ItemInView = GetUsableInView();
	if (ItemInView) {

		if (ItemInView->bCanBePickedUp) {
			PickUpItem(ItemInView);
			ItemInView->PickedUp();
		}
		else {
			ItemInView->OnUsed();
		}
	}
}


void ACommissarCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}


void ACommissarCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = false;
}


void ACommissarCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}


void ACommissarCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkingSpeed * 2.0f;
}


void ACommissarCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxWalkingSpeed;
}


void ACommissarCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}


void ACommissarCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}


void ACommissarCharacter::StartCrouch()
{
	if (CanCrouch() == true)
	{
		Crouch();
	}
}


void ACommissarCharacter::EndCrouch()
{
	UnCrouch();
}


void ACommissarCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}


void ACommissarCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


ACommissarItem* ACommissarCharacter::GetUsableInView() {
	FVector CamLoc;
	FRotator CamRot;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("doing"));

	if (Controller == NULL)
		return NULL;

	/* This retrieves are camera point of view to find the start and direction we will trace. */
	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = CamLoc;
	const FVector Direction = CamRot.Vector();
	const FVector TraceEnd = TraceStart + (Direction * MaxUseDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("TraceUsableActor")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;
	TraceParams.bTraceComplex = true;

	/* FHitResults is passed in with the trace function and holds the result of the trace. */
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);

	/* Uncomment this to visualize your line during gameplay. */
	//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f);

	return Cast<ACommissarItem>(Hit.GetActor());
}

void ACommissarCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}


// Item interaction logic

void ACommissarCharacter::PickUpItem(ACommissarItem* NewItem) {
	if (NewItem) {
		ItemInventory.Add(NewItem);
		NewItem->PickedUp();
	}
}


TArray<class ACommissarItem*> ACommissarCharacter::GetInventory() {
	return ItemInventory;
}


void ACommissarCharacter::ToggleInventory() {
	if (!bIsInventoryOpen) {
		bIsInventoryOpen = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inventory open"));
		TArray<class ACommissarItem*> CurrentInventory = this->GetInventory();

		for (auto& Item : CurrentInventory) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item name: %s"), *Item->ItemName));
		}
	}
	else {
		bIsInventoryOpen = false;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inventory closed"));
	}
}


// Player character stat logic
int ACommissarCharacter::GetAttributeValue(FString AttributeName) {

		if (AttributeName == "Health") return Health;
		if (AttributeName == "Shields") return Shields;
		if (AttributeName == "Credits") return Credits;
		if (AttributeName == "Matter") return Matter;

		return false;
}

void ACommissarCharacter::SetAttributeValue(FString AttributeName, int Amount) {
	int Attribute;

	if (AttributeName == "Health") Attribute = Health;
	if (AttributeName == "Shields") Attribute = Shields;
	if (AttributeName == "Credits") Attribute = Credits;
	if (AttributeName == "Matter") Attribute = Matter;

	if (Amount <= MaxHealth && Amount >= 0) 
	{
		Attribute = Amount;
	}
	else
	{
		if (Amount > MaxHealth)
		{
			Attribute = MaxHealth;
		}

		if (Amount < 0)
		{
			Attribute = 0;
		}
	}
}