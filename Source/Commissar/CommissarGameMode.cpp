// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Commissar.h"
#include "CommissarGameMode.h"
#include "CommissarHUD.h"
#include "CommissarCharacter.h"

ACommissarGameMode::ACommissarGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACommissarHUD::StaticClass();
}
