// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "CommissarHUD.generated.h"

UCLASS()
class ACommissarHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACommissarHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	void BeginPlay() override;

	/** Set up connection to UMG widgets */
	class UClass* ActiveHUDWidgetClass;
	class UUserWidget* ActiveHUDWidget;

	class UClass* PauseWidgetClass;
	class UUserWidget* PauseWidget;

	class UClass* CharacterHUDWidgetClass;
	class UUserWidget* CharacterHUDWidget;

private:

};

