// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperCharacterEnemyBase.generated.h"

class UPaperFlipbook;

UENUM()
enum class EEnemyMovementState:uint8
{
	Idle,
	Walk,
	Attack,
	Hurt,
	Death
};

class APaperCharacterEnemyBase;

typedef bool (APaperCharacterEnemyBase::*ConditionFuncPtrType)(void);

USTRUCT()
struct FEnemyMovementChanger
{
	GENERATED_BODY()
	
	EEnemyMovementState TargetState;
	bool bIsLoop;
	ConditionFuncPtrType ConditionFunc;
};

/**
 * 
 */
UCLASS()
class CPP2DGAME_API APaperCharacterEnemyBase : public APaperCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bIsAttacking;
	UPROPERTY()
	bool bIsHiting;
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP;
	UPROPERTY(EditAnywhere)
	float Damage;
	
	UPROPERTY(EditAnywhere)
	EEnemyMovementState CurrentEnemyMovementState;
	
	UPROPERTY(EditAnywhere, Category="AnimationMap")
	TMap<EEnemyMovementState, UPaperFlipbook*> EnemyAnimationMap;
	
	TMap<EEnemyMovementState, TArray<FEnemyMovementChanger>> MovementStateDataMap;

	FEnemyMovementChanger ToIdle;
	FEnemyMovementChanger ToWalk;
	FEnemyMovementChanger ToAttack;
	FEnemyMovementChanger ToHit;
	FEnemyMovementChanger ToDeath;
	
	APaperCharacterEnemyBase();

	UFUNCTION(Category = "CallBack")
	void OnSpriteFinshPlaying();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void ChangeEnemyMovementState(EEnemyMovementState NewMovementState, bool bIsLoop);

	UFUNCTION(BlueprintCallable)
	virtual void OnAttack();
	virtual void OnAttackEnd();

	virtual void OnHit(float Damage);
	virtual void OnHitEnd();
	
	virtual void OnDead();
	UFUNCTION(BlueprintImplementableEvent)
	void OnDead_Implement();
	
	bool GetIsWalking();
	bool GetIsStop();
	bool GetIsAttacking();
	bool GetIsHiting();
	bool GetIsDead();
};
