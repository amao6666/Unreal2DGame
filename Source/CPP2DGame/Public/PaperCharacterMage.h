// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacterBase.h"
#include "PaperCharacterMage.generated.h"

/**
 * 
 */
UCLASS()
class CPP2DGAME_API APaperCharacterMage : public APaperCharacterBase
{
	GENERATED_BODY()

public:
    APaperCharacterMage();
	UPROPERTY(EditAnywhere)
	USceneComponent* BulletSocket;
	UPROPERTY(EditAnywhere)
	USceneComponent* SkillSocket;

	virtual void OnAttack() override;
	virtual void OnSkill() override;
	bool CheckAttackOrSkill();
	void GenerateAttackBullet();
	void GenerateSkillBullet();
};