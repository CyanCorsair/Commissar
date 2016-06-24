// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Commissar.h"
#include "CommissarHUD.h"
#include "CommissarCharacter.h"
#include "Engine/Canvas.h"
#include "Engine.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "SMainHudWidget.h"

ACommissarHUD::ACommissarHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshiarTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshiarTexObj.Object;
	CurrentHealth = 100;
}


void ACommissarHUD::DrawHUD()
{
	Super::DrawHUD();
	UpdateHealth();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}


void ACommissarHUD::BeginPlay() {
	Super::BeginPlay();

	MainHudWidget = SNew(SMainHudWidget).OwnerHUD(this);
	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MainHudWidget.ToSharedRef()));
	MainHudWidget->SetVisibility(EVisibility::Visible);
}

void ACommissarHUD::UpdateHealth() {
	ACommissarCharacter* CurrentCharacter = Cast<ACommissarCharacter>(GetOwningPawn());

	if (CurrentCharacter) {
		CurrentHealth = CurrentCharacter->GetHealth();
	}
}