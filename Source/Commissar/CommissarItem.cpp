// Fill out your copyright notice in the Description page of Project Settings.

#include "Commissar.h"
#include "CommissarItem.h"


// Sets default values
ACommissarItem::ACommissarItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>( TEXT("Mesh"));
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
	
}

void ACommissarItem::PickedUp() {
	if (Mesh) {
		Mesh->DestroyComponent();
	}
}

void ACommissarItem::Dropped() {

}