// Fill out your copyright notice in the Description page of Project Settings.

#include "Commissar.h"
#include "CommissarAmmunition.h"

#include "CommissarProjectile.h"

#include "Engine.h"

ACommissarAmmunition::ACommissarAmmunition()
{
	bIsProjectile = true;
	CapacityPerUse = 200;
	CurrentCapacity = CapacityPerUse;
}

void ACommissarAmmunition::ReduceCurrentCapacity()
{
	if (CurrentCapacity >= 1) CurrentCapacity--;
}

void ACommissarAmmunition::RemoveSelfOnEmpty()
{
	if (CurrentCapacity == 0) Destroy();
}