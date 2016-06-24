// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CommissarItem.generated.h"

UCLASS()
class COMMISSAR_API ACommissarItem : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	bool bCanBePickedUp;

	// Sets default values for this actor's properties
	ACommissarItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Interaction
	void OnUsed();
	void PickedUp();
	void Dropped();

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
