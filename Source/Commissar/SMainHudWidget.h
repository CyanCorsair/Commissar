// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class COMMISSAR_API SMainHudWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainHudWidget)
	{}

	/*See private declaration of OwnerHUD below.*/
	SLATE_ARGUMENT(TWeakObjectPtr<class ACommissarHUD>, OwnerHUD)

	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TWeakObjectPtr<class ACommissarHUD> OwnerHUD;

	FText GetStringFromInt(int InputNumber);
};
