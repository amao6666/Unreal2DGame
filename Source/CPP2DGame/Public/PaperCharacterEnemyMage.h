// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacterEnemyBase.h"
#include "PaperCharacterEnemyMage.generated.h"

/**
 * 
 */
UCLASS()
class CPP2DGAME_API APaperCharacterEnemyMage : public APaperCharacterEnemyBase
{
	GENERATED_BODY()
public:
	virtual void OnAttack() override;
};
