// Fill out your copyright notice in the Description page of Project Settings.

#include "Commissar.h"
#include "CommissarItem.h"
#include "CommissarCharacter.h"


// Sets default values
ACommissarItem::ACommissarItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DefaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh = DefaultMesh;
	RootComponent = Mesh;
	bCanBePickedUp = true;
}

// Called when the game starts or when spawned
void ACommissarItem::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetSimulatePhysics(true);
	Mesh->WakeRigidBody();
}

// Called every frame
void ACommissarItem::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ACommissarItem::OnUsed() {
	ACommissarCharacter* Owner = Cast<ACommissarCharacter>(GetOwner());

	if (Owner)
	{

	}
}

void ACommissarItem::PickedUp() {
	if (Mesh)
	{
		Mesh->DestroyComponent();
	}
}

void ACommissarItem::Dropped() {
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

	Mesh = DefaultMesh;
}