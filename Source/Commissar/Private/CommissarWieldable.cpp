// Fill out your copyright notice in the Description page of Project Settings.

#include "Commissar.h"
#include "CommissarWieldable.h"


// Sets default values
ACommissarWieldable::ACommissarWieldable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACommissarWieldable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommissarWieldable::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

