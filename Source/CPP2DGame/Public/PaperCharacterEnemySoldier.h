// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacterEnemyBase.h"
#include "Components/BoxComponent.h"
#include "PaperCharacterEnemySoldier.generated.h"

/**
 * 
 */
UCLASS()
class CPP2DGAME_API APaperCharacterEnemySoldier : public APaperCharacterEnemyBase
{
	GENERATED_BODY()

public:
	APaperCharacterEnemySoldier();
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* WeaponBox;
	UPROPERTY(EditAnywhere)
	UPaperFlipbookComponent* WeaponSprite;

	virtual void BeginPlay() override;
	
	virtual void OnAttack() override;
	virtual void OnAttackEnd() override;
	virtual void OnDead() override;

	UFUNCTION()
	void OnMagicEnd();

	UFUNCTION()
	void OnWeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
