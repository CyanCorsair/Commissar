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
	static ConstructorHelpers::FClassFinder<UUserWidget> TestHUDWidget(TEXT("/Game/UIBP/MainHUD"));
	if (TestHUDWidget.Succeeded()) {
		TestHUDWidgetClass = TestHUDWidget.Class;
	}
	else {
		// hudWidgetObj not found
		TestHUDWidgetClass = nullptr;
	}
}


void ACommissarHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ACommissarHUD::BeginPlay() {
	Super::BeginPlay();

	if (TestHUDWidgetClass) {
		// the player controller should be constructed by now so we can get a reference to it
		TestHUDWidget = CreateWidget<UUserWidget>(this->GetOwningPlayerController(), this->TestHUDWidgetClass);
		TestHUDWidget->AddToViewport();
	}
}