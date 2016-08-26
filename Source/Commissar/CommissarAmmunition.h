// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommissarConsumableItem.h"
#include "CommissarAmmunition.generated.h"

/**
 * 
 */
UCLASS()
class COMMISSAR_API ACommissarAmmunition : public ACommissarConsumableItem
{
	GENERATED_BODY()

// Properties
public:
	ACommissarAmmunition();

	bool bIsProjectile;

// Methods
public:
};
