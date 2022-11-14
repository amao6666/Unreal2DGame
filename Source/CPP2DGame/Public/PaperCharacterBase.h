// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "PaperCharacterBase.generated.h"

class UPaperFlipbook;

UENUM()
enum class EHeroMovementState:uint8
{
	Idle,
	Walk,
	Run,
	
	Attack,
	Walk_Attack,
	Run_Attack,

	Skill,
	
	Jump_Up,
	Jump_Down,
	Jump_Land,
	
	High_Jump_Up,
	High_Jump_Down,
	
	Climb,
	Climb_Idle,
	
	Hurt,
	Push,
	Death
};

enum class EStaticItemType:uint8;

class APaperCharacterBase;

typedef bool (APaperCharacterBase::*ConditionFuncPtrType)(void);

USTRUCT()
struct FMovementStateChanger
{
	GENERATED_BODY()
	
	EHeroMovementState TargetState;
	bool bIsLoop;
	ConditionFuncPtrType ConditionFunc;
};

/**
 * 
 */
UCLASS()
class CPP2DGAME_API APaperCharacterBase : public APaperCharacter
{
	GENERATED_BODY()

public:
	UPROPERTY();
	bool bIsSkilling;
	bool bIsAttacking;
	bool bIsHiting;
	bool bCanClimb;
	bool bIsClimbing;
	bool bIsInClimb;
	bool bIsOnClimbTop;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP;
	UPROPERTY(EditAnywhere)
	float Damage;
	UPROPERTY(BlueprintReadOnly)
	int32 CoinNumber;

	class AClimbActor* CanClimbActor;

	UPROPERTY(EditAnywhere)
	float WalkSpeed;
	UPROPERTY(EditAnywhere)
	float RunSpeed;
	
	UPROPERTY(EditAnywhere)
	EHeroMovementState CurrentHeroMovementState;
	
	UPROPERTY(EditAnywhere, Category="AnimationMap")
	TMap<EHeroMovementState, UPaperFlipbook*> HeroAnimationMap;
	
	int32 CurrentJumpLevel;
	
	TMap<EHeroMovementState, TArray<FMovementStateChanger>> MovementStateMochiniDataMap;

	FMovementStateChanger ToIdle;
	FMovementStateChanger ToWalk;
	FMovementStateChanger ToRun;
	FMovementStateChanger ToAttack;
	FMovementStateChanger ToHit;
	FMovementStateChanger ToSkill;
	FMovementStateChanger ToJumpUp;
	FMovementStateChanger ToJumpDown;
	FMovementStateChanger ToHighJumpUp;
	FMovementStateChanger ToHighJumpDown;
	FMovementStateChanger ToJumpLand;
	FMovementStateChanger ToWalkAttack;
	FMovementStateChanger ToRunAttack;
	FMovementStateChanger ToClimb;
	FMovementStateChanger ToClimbIdle;

	APaperCharacterBase();
	
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void OnMove(float AxisValue);
	void OnClimb(float AxisValue);
	void OnJump();
	virtual void OnAttack();
	virtual void OnSkill();
	
	virtual void OnHit(float Damage);
	virtual void OnHitEnd();

	virtual void OnDead();
	virtual void OnDeadEnd();
	
	void OnSpeedUp();
	void OnSpeedDown();

	void MovementStateMochini(float AxisValue);
	
	void ChangeHeroMovementState(EHeroMovementState NewMovementState, bool bIsLoop);

	UFUNCTION(Category = "CallBack")
	void OnSpriteFinshPlaying();

	void OnBeginOverlapClimb(class AClimbActor* ClimbActor);
	void OnEndOverlapClimb(class AClimbActor* ClimbActor);

	void OnBeginOverlapStaticItem(EStaticItemType ItemType);

	bool GetIsWalking();
	bool GetIsRuning();
	bool GetIsFalling();
	bool GetIsFallingUp();
	bool GetIsFallingDown();
	bool GetIsHighJumpUp();
	bool GetIsLand();
	bool GetIsStop();
	bool GetIsAttacking();
	bool GetIsSkilling();
	bool GetIsAnimPlayEnd();
	bool GetIsClimbing();
	bool GetIsClimbIdle();
	bool GetIsHiting();
};