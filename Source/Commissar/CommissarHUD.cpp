// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Commissar.h"
#include "CommissarHUD.h"
#include "CommissarCharacter.h"
#include "Engine.h"

#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

ACommissarHUD::ACommissarHUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ActiveHUDWidget(TEXT("/Game/UIBP/MainHUD"));
	if (ActiveHUDWidget.Succeeded()) {
		ActiveHUDWidgetClass = ActiveHUDWidget.Class;
	}
	else {
		// hudWidgetObj not found
		ActiveHUDWidgetClass = nullptr;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseWidget(TEXT("/Game/UIBP/PauseMenu"));
	if (PauseWidget.Succeeded()) {
		PauseWidgetClass = PauseWidget.Class;
	}
	else {
		// hudWidgetObj not found
		PauseWidgetClass = nullptr;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> CharacterHUDWidget(TEXT("/Game/UIBP/CharacterMenu"));
	if (CharacterHUDWidget.Succeeded()) {
		CharacterHUDWidgetClass = CharacterHUDWidget.Class;
	}
	else {
		// hudWidgetObj not found
		CharacterHUDWidgetClass = nullptr;
	}
}


void ACommissarHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ACommissarHUD::BeginPlay() {
	Super::BeginPlay();

	if (ActiveHUDWidgetClass) {
		// the player controller should be constructed by now so we can get a reference to it
		ActiveHUDWidget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(), this->ActiveHUDWidgetClass);
		ActiveHUDWidget->AddToViewport();
	}

	if (PauseWidgetClass) {
		// the player controller should be constructed by now so we can get a reference to it
		PauseWidget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(), this->PauseWidgetClass);
		PauseWidget->AddToViewport();
	}

	if (CharacterHUDWidgetClass) {
		// the player controller should be constructed by now so we can get a reference to it
		CharacterHUDWidget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(), this->CharacterHUDWidgetClass);
		CharacterHUDWidget->AddToViewport();
	}
}