// Fill out your copyright notice in the Description page of Project Settings.

#include "Commissar.h"
#include "CommissarWearable.h"


// Sets default values
ACommissarWearable::ACommissarWearable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACommissarWearable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommissarWearable::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

