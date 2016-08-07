// Fill out your copyright notice in the Description page of Project Settings.

#include "Commissar.h"
#include "CommissarBaseSkill.h"

UCommissarBaseSkill::UCommissarBaseSkill()
{
	/** Skill UI display name and shorthand */
	FriendlyName = "Base Skill";
	SystemName = "skill_Base";

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
}

int UCommissarBaseSkill::SetCurrentSkillLevel() {
	return 0;
}

int UCommissarBaseSkill::SetExperienceCost() {
	return 0;
}

void UCommissarBaseSkill::UpdateSkillEffects() {

}