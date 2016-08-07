// Fill out your copyright notice in the Description page of Project Settings.

#include "Commissar.h"
#include "CommissarConsumableMedkitItem.h"
#include "CommissarCharacter.h"
#include "CommissarMedicineSkill.h"

#include "Engine.h"

ACommissarConsumableMedkitItem::ACommissarConsumableMedkitItem()
{
	int AmountHealed = 25;
}

void ACommissarConsumableMedkitItem::OnUsed()
{
	ACommissarCharacter* Owner = Cast<ACommissarCharacter>(GetOwner());

	if (Owner)
	{
		int CurrentHealth = Owner->GetAttributeValue("Health");
		UCommissarMedicineSkill* MedicineSkill = Cast<UCommissarMedicineSkill>(Owner->SkillList[0]);

		int NewHealth = CurrentHealth + (AmountHealed * MedicineSkill->MedkitHealingModifier);
		Owner->SetAttributeValue("Health", NewHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Amount to be healed: %s"), NewHealth));
	}
}