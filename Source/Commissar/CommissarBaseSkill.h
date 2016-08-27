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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
		FString FriendlyName;

	/** Skill description strings */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
		FString UntrainedDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
		FString TrainedDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
		FString AdvancedDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
		FString MasterDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
		int ExperienceCost;

	/** Current level of skill */
	int CurrentLevel;

	/** Icon sprites */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
		UTexture2D* IconLarge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Assets)
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
