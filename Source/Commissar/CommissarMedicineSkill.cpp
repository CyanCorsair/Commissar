// Fill out your copyright notice in the Description page of Project Settings.

#include "Commissar.h"
#include "CommissarMedicineSkill.h"

UCommissarMedicineSkill::UCommissarMedicineSkill()
{
	FriendlyName = "Medicine";
	SystemName = "skill_Medicine";

	/** Skill description strings */
	UntrainedDescription = "Skill effects at lowest level.";

	TrainedDescription = "Skill at second lowest level.";

	AdvancedDescription = "Skill at second highest level.";

	MasterDescription = "Skill at highest level.";

	ExperienceCost = 250;

	/** Current level of skill */
	CurrentLevel = 1;

	/** Experience cost integers */
	BaseExperienceCost = 250;
	ExperienceCostModifier = 0.5f;

	/** Does the character have access? */
	bHasAccess = true;

	/** Skill effects */
	int MedkitHealingModifier = 1; // Integer multiplier
}

int UCommissarMedicineSkill::SetCurrentSkillLevel() {
	return 0;
}

int UCommissarMedicineSkill::SetExperienceCost() {
	return 0;
}

void UCommissarMedicineSkill::UpdateSkillEffects() {

}


