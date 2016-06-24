// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "SMainHudWidget.h"
#include "CommissarHUD.generated.h"

UCLASS()
class ACommissarHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACommissarHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	/** Slate widget declarations */
	TSharedPtr<SMainHudWidget> MainHudWidget;

	void BeginPlay() override;
	void UpdateHealth();

	// State variables
	int CurrentHealth;
	FString CurrentlyHeldItem;


private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

