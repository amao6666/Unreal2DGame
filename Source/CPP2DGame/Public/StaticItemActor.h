// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "StaticItemActor.generated.h"

UENUM()
enum class EStaticItemType:uint8
{
	Coin, Heart, BloodPag, Trap
};

/**
 * 
 */
UCLASS()
class CPP2DGAME_API AStaticItemActor : public APaperFlipbookActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EStaticItemType ItemType;

	UPROPERTY(EditAnywhere)
	FVector2D BoxCollisionSize;

	UPROPERTY(EditAnywhere)
	FVector2D BoxCollisionPosition;

	class UBoxComponent* BoxCollision;
	
	AStaticItemActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnBoxCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnAfterHit(AActor* OtherActor);
};
