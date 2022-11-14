// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StaticItemActor.h"
#include "FliptrapItemActor.generated.h"

/**
 * 
 */
UCLASS()
class CPP2DGAME_API AFliptrapItemActor : public AStaticItemActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	class UPaperFlipbook* NormalSprite;

	UPROPERTY(EditAnywhere)
	class UPaperFlipbook* MovementSprite;

	UPROPERTY(EditAnywhere)
	float DelaySecond;
	
	float CurrentTime;
	
	AFliptrapItemActor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void OnMovementStart();
	
	UFUNCTION()
	void OnMovementPlayFinished();
};
