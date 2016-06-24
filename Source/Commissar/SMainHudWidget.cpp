// Fill out your copyright notice in the Description page of Project Settings.

#include "Commissar.h"
#include "SMainHudWidget.h"
#include "SlateOptMacros.h"
#include "CommissarHUD.h"

#define LOCTEXT_NAMESPACE "SMainHudWidget"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMainHudWidget::Construct(const FArguments& InArgs)
{
	OwnerHUD = InArgs._OwnerHUD;
	
	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
	[
		// Populate the widget
		SNew(SOverlay)
		+SOverlay::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.ShadowColorAndOpacity(FLinearColor::Black)
			.ColorAndOpacity(FLinearColor::Red)
			.ShadowOffset(FIntPoint(-1, 1))
			.Font(FSlateFontInfo("Verdana", 16))
			.Text(GetStringFromInt(OwnerHUD->CurrentHealth))
		]
	];
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


FText SMainHudWidget::GetStringFromInt(int InputNumber) {
	return FText::FromString(FString::FromInt(InputNumber));
}