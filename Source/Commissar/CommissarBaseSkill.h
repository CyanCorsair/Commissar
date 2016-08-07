// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "CommissarBaseSkill.generated.h"

/**
 * 
 */
UCLASS()
class COMMISSAR_API UCommissarBaseSkill : public UObject
{
	GENERATED_BODY()

public:
	UCommissarBaseSkill();

	/** Skill UI display name and shorthand */
	UPROPERTY()
		FString FriendlyName;

	/** Skill description strings */
	UPROPERTY()
		FString UntrainedDescription;

	UPROPERTY()
		FString TrainedDescription;

	UPROPERTY()
		FString AdvancedDescription;

	UPROPERTY()
		FString MasterDescription;

	UPROPERTY()
		int ExperienceCost;

	/** Current level of skill */
	int CurrentLevel;

	/** Icon sprites */
	UPROPERTY()
		UTexture2D* IconLarge;

	UPROPERTY()
		UTexture2D* IconSmall;

	/**
	* Does the owner have access?
	* Used for Advanced Training
	*/
	UPROPERTY()
		bool bHasAccess;

	int SetCurrentSkillLevel();

protected:
	FString SystemName;

	/** Experience cost integers */
	int BaseExperienceCost;
	float ExperienceCostModifier;

	/** Accessor methods */
	int SetExperienceCost();
	void UpdateSkillEffects();
};
