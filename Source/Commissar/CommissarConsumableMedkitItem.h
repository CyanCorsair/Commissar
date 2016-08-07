// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommissarConsumableItem.h"
#include "CommissarConsumableMedkitItem.generated.h"

/**
 * 
 */
UCLASS()
class COMMISSAR_API ACommissarConsumableMedkitItem : public ACommissarConsumableItem
{
	GENERATED_BODY()

		int AmountHealed;

public:
	// Sets default values for this actor's properties
	ACommissarConsumableMedkitItem();

	// Interaction
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void OnUsed();
	
};
