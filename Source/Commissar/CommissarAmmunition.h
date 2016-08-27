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
	bool bIsProjectile;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Statistic)
	int CapacityPerUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Statistic)
	int CurrentCapacity;

// Methods
public:
	ACommissarAmmunition();

	void ReduceCurrentCapacity();
	void RemoveSelfOnEmpty();
};
