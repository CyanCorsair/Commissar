// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommissarItem.h"
#include "GameFramework/Actor.h"
#include "CommissarWearable.generated.h"

UCLASS()
class COMMISSAR_API ACommissarWearable : public ACommissarItem
{
	GENERATED_BODY()

// Properties
public:
	int MaxShieldCount;

// Methods
public:	
	// Sets default values for this actor's properties
	ACommissarWearable();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
