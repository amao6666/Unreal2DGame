// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticItemActor.h"
#include "FlyItemActor.generated.h"

/**
 * 
 */
UCLASS()
class CPP2DGAME_API AFlyItemActor : public AStaticItemActor
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere)
	class UPaperFlipbook*  HitSprite;

	class UPaperFlipbook*  FlySprite;

	UPROPERTY(EditAnywhere)
	float DelaySecond;

	UPROPERTY(EditAnywhere)
	FVector Direction;

	FVector StartLocation;
	bool bCanMove;
	bool bIsHited;
	float CurrentTime;
	
	AFlyItemActor();

	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaSeconds) override;

	void OnStartMove();
	
	UFUNCTION()
	void OnHitPlayFinished();
	
	void OnAfterHit(AActor* OtherActor);

	void SpawnSelfToStartLocation();
};
