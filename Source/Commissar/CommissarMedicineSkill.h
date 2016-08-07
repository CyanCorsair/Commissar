// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CommissarBaseSkill.h"
#include "CommissarMedicineSkill.generated.h"

/**
 * 
 */
UCLASS()
class COMMISSAR_API UCommissarMedicineSkill : public UCommissarBaseSkill
{
	GENERATED_BODY()


public:
	UCommissarMedicineSkill();

	int SetCurrentSkillLevel();

	int MedkitHealingModifier;

protected:
	int SetExperienceCost();
	void UpdateSkillEffects();
};
