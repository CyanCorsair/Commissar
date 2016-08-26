// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Commissar.h"
#include "CommissarCharacter.h"
#include "CommissarItem.h"
#include "CommissarConsumableItem.h"
#include "CommissarWieldable.h"
#include "CommissarWearable.h"

#include "CommissarBaseSkill.h"

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
	InventoryGridSquareSize = 64;
	InventoryGridX, InventoryGridY = 16;
	MaxUseDistance = 200.f;

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

	// Character attribute setup
	MaxHealth = 100, Health = 100;
	MaxCredits = 1000000, Credits = 250;
	MaxMatter = 2480, Matter = 100;
	MaxShields = 0, Shields = 0;

	DisplayHealth = Health;
	DisplayShields = Shields;
	DisplayCredits = Credits;
	DisplayMatter = Matter;

	bHasNewFocus = true;
	CurrentlyHeld = NULL;
	CurrentlyWorn = NULL;

	if (CurrentlyWorn != NULL)
	{
		MaxShields = CurrentlyWorn->MaxShieldCount;
		Shields = MaxShields;
	}
}


void ACommissarCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	CurrentState = ECharacterState::Idle;
	CurrentMovementState = ECharacterMovementState::Stopped;

	DefaultMaxWalkingSpeed = GetCharacterMovement()->MaxWalkSpeed;

	FActorSpawnParameters HeldSpawnParams;
	HeldSpawnParams.Owner = this;

	if (CurrentlyHeld)
	{
		Mesh1P->SetVisibility(true);
		//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
		CurrentlyHeld->Mesh->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	else
	{
		Mesh1P->SetVisibility(false);
	}
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
	if (CurrentlyHeld) CurrentlyHeld->OnUsed();
}


void ACommissarCharacter::OnUse() {
	ACommissarItem* ItemInView = GetUsableInView();
	if (ItemInView) {

		if (ItemInView->bCanBePickedUp) {
			PickUpItem(ItemInView);
			ItemInView->PickedUp();
			if (ItemInView->IsA(ACommissarWieldable::StaticClass()) && CurrentlyHeld == NULL)
			{
				CurrentlyHeld = Cast<ACommissarWieldable>(ItemInView);
				Mesh1P->SetVisibility(true);
				CurrentlyHeld->SetOwner(this);
				CurrentlyHeld->WieldableMesh->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
			}
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


ACommissarItem* ACommissarCharacter::GetUsableInView()
{
	FVector CamLoc;
	FRotator CamRot;

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

void ACommissarCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ACommissarItem* Usable = GetUsableInView();

	// End focus
	if (FocusedUsableActor != Usable)
	{
		if (FocusedUsableActor)
		{
			FocusedUsableActor->OnEndFocus();
		}

		bHasNewFocus = true;
	}

	// Assign new focus
	FocusedUsableActor = Usable;

	if (Usable)
	{
		if (bHasNewFocus)
		{
			Usable->OnBeginFocus();
			bHasNewFocus = false;
		}
	}
}


// Item interaction logic

void ACommissarCharacter::PickUpItem(ACommissarItem* NewItem)
{
	if (NewItem) {
		ItemInventory.Add(NewItem);
		NewItem->PickedUp();
	}
}


TArray<class ACommissarItem*> ACommissarCharacter::GetInventory()
{
	return ItemInventory;
}


void ACommissarCharacter::ToggleInventory()
{
	if (CurrentState != ECharacterState::UsingCharacterMenu)
	{
		CurrentState = ECharacterState::UsingCharacterMenu;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inventory open"));
		TArray<class ACommissarItem*> CurrentInventory = this->GetInventory();

		for (auto& Item : CurrentInventory)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Item name: %s"), *Item->ItemName));
		}
	}
	else
	{
		CurrentState = ECharacterState::Idle;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inventory closed"));
	}
}


// Player character stat logic
int ACommissarCharacter::GetAttributeValue(FString AttributeName)
{

	if (AttributeName == "Health") return Health;
	if (AttributeName == "Shields") return Shields;
	if (AttributeName == "Credits") return Credits;
	if (AttributeName == "Matter") return Matter;

	return false;
}

void ACommissarCharacter::SetAttributeValue(FString AttributeName, int Amount)
{
	if (AttributeName == "Health") Health = CalculateNewValue(MaxHealth, Amount);
	if (AttributeName == "Shields") Shields = CalculateNewValue(MaxShields, Amount);
	if (AttributeName == "Credits") Credits = CalculateNewValue(MaxCredits, Amount);
	if (AttributeName == "Matter") Matter = CalculateNewValue(MaxMatter, Amount);
}
	

int ACommissarCharacter::CalculateNewValue(int Max, int New)
{
	int Attribute = 0;

	if (New <= Max && New >= 0)
	{
		Attribute = New;
	}
	else
	{
		if (New > Max)
		{
			Attribute = Max;
		}

		if (New < 0)
		{
			Attribute = 0;
		}
	}

	return Attribute;
}