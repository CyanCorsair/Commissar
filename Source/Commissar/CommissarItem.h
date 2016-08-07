// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "CommissarItem.generated.h"

UCLASS()
class COMMISSAR_API ACommissarItem : public AActor
{
	GENERATED_BODY()
	
	// Properties
public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
		int32 Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UStaticMeshComponent* DefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
		bool bCanBePickedUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Wieldable)
		bool bHasInfiniteUses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Wieldable)
		bool bHasInfiniteMaxUses;

	// In other words how many shots are in one "clip"
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Wieldable)
		int MaxUses;

	// Count of how many uses are left
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Wieldable)
		int Uses;

	// Methods
public:
	// Sets default values for this actor's properties
	ACommissarItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Interaction
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void OnUsed();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PickedUp();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void Dropped();

	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

};
