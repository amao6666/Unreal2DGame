// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperCharacterMage.h"
#include "PaperFlipbookComponent.h"
#include "BulletMageAttackActor.h"
#include "BulletMageSkillActor.h"

APaperCharacterMage::APaperCharacterMage()
{
	BulletSocket = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSocket"));
	SkillSocket = CreateDefaultSubobject<USceneComponent>(TEXT("SkillSocket"));
	BulletSocket->SetupAttachment(GetSprite());
	SkillSocket->SetupAttachment(GetSprite());
	
	TArray<FMovementStateChanger> IdleArray;
    IdleArray.Add(ToJumpUp);
	IdleArray.Add(ToWalk);
	IdleArray.Add(ToRun);
	IdleArray.Add(ToAttack);
	IdleArray.Add(ToHit);
	IdleArray.Add(ToSkill);
	IdleArray.Add(ToClimb);
    MovementStateMochiniDataMap.Add(EHeroMovementState::Idle, IdleArray);

	TArray<FMovementStateChanger> JumpUpArray;
	JumpUpArray.Add(ToHighJumpUp);
	JumpUpArray.Add(ToJumpDown);
	JumpUpArray.Add(ToJumpLand);
	JumpUpArray.Add(ToClimb);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Jump_Up, JumpUpArray);

	TArray<FMovementStateChanger> JumpDownArray;
	JumpDownArray.Add(ToHighJumpUp);
	JumpDownArray.Add(ToJumpLand);
	JumpDownArray.Add(ToClimb);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Jump_Down, JumpDownArray);

	TArray<FMovementStateChanger> HighJumpUpArray;
	HighJumpUpArray.Add(ToHighJumpDown);
	HighJumpUpArray.Add(ToJumpLand);
	HighJumpUpArray.Add(ToClimb);
	MovementStateMochiniDataMap.Add(EHeroMovementState::High_Jump_Up, HighJumpUpArray);

	TArray<FMovementStateChanger> HighJumpDownArray;
	HighJumpDownArray.Add(ToJumpLand);
	HighJumpDownArray.Add(ToClimb);
	MovementStateMochiniDataMap.Add(EHeroMovementState::High_Jump_Down, JumpDownArray);

	TArray<FMovementStateChanger> ClimbArray;
	ClimbArray.Add(ToClimbIdle);
	ClimbArray.Add(ToHighJumpUp);
	ClimbArray.Add(ToJumpDown);
	ClimbArray.Add(ToJumpLand);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Climb, ClimbArray);

	TArray<FMovementStateChanger> ClimbIdleArray;
	ClimbIdleArray.Add(ToClimb);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Climb_Idle, ClimbIdleArray);

	TArray<FMovementStateChanger> JumpLandArray;
	JumpLandArray.Add(ToIdle);
	JumpLandArray.Add(ToWalk);
	JumpLandArray.Add(ToRun);
	JumpLandArray.Add(ToJumpUp);
	JumpLandArray.Add(ToJumpDown);
	JumpLandArray.Add(ToAttack);
	JumpLandArray.Add(ToHit);
	JumpLandArray.Add(ToSkill);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Jump_Land, JumpLandArray);
	
	TArray<FMovementStateChanger> WalkArray;
	WalkArray.Add(ToSkill);
	WalkArray.Add(ToWalkAttack);
	WalkArray.Add(ToHit);
	WalkArray.Add(ToJumpUp);
	WalkArray.Add(ToJumpDown);
	WalkArray.Add(ToRun);
	WalkArray.Add(ToIdle);
	WalkArray.Add(ToClimb);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Walk, WalkArray);
	
	TArray<FMovementStateChanger> RunArray;
	RunArray.Add(ToSkill);
	RunArray.Add(ToRunAttack);
	RunArray.Add(ToHit);
    RunArray.Add(ToJumpUp);
	RunArray.Add(ToJumpDown);
	RunArray.Add(ToWalk);
    RunArray.Add(ToIdle);
	RunArray.Add(ToClimb);
    MovementStateMochiniDataMap.Add(EHeroMovementState::Run, RunArray);

	TArray<FMovementStateChanger> AttackArray;
	AttackArray.Add(ToIdle);
	AttackArray.Add(ToWalk);
	AttackArray.Add(ToRun);
	AttackArray.Add(ToJumpUp);
	AttackArray.Add(ToJumpDown);
	AttackArray.Add(ToAttack);
	AttackArray.Add(ToSkill);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Attack, AttackArray);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Skill, AttackArray);

	TArray<FMovementStateChanger> HurtArray;
	HurtArray.Add(ToIdle);
	HurtArray.Add(ToWalk);
	HurtArray.Add(ToRun);
	HurtArray.Add(ToJumpUp);
	HurtArray.Add(ToAttack);
	HurtArray.Add(ToSkill);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Hurt, AttackArray);

	TArray<FMovementStateChanger> WalkAttackArray;
	WalkAttackArray.Add(ToIdle);
	WalkAttackArray.Add(ToWalk);
	WalkAttackArray.Add(ToRun);
	WalkAttackArray.Add(ToJumpUp);
	WalkAttackArray.Add(ToJumpDown);
	WalkAttackArray.Add(ToWalkAttack);
	WalkAttackArray.Add(ToHit);
	WalkAttackArray.Add(ToClimb);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Walk_Attack, WalkAttackArray);

	TArray<FMovementStateChanger> RunAttackArray;
	RunAttackArray.Add(ToIdle);
	RunAttackArray.Add(ToWalk);
	RunAttackArray.Add(ToRun);
	RunAttackArray.Add(ToJumpUp);
	RunAttackArray.Add(ToJumpDown);
	RunAttackArray.Add(ToRunAttack);
	RunAttackArray.Add(ToHit);
	RunAttackArray.Add(ToClimb);
	MovementStateMochiniDataMap.Add(EHeroMovementState::Run_Attack, WalkAttackArray);
}

void APaperCharacterMage::OnAttack()
{
	Super::OnAttack();
	if (!CheckAttackOrSkill()) return;
	bIsAttacking = true;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APaperCharacterMage::GenerateAttackBullet, 0.3f, false);
}

void APaperCharacterMage::OnSkill()
{
	Super::OnSkill();
	if (!CheckAttackOrSkill()) return;
	bIsSkilling = true;
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APaperCharacterMage::GenerateSkillBullet, 0.3f, false);
}

bool APaperCharacterMage::CheckAttackOrSkill()
{
	return !(GetIsFalling() || GetIsAttacking() || GetIsSkilling() || bIsInClimb || bIsHiting) && BulletSocket;
}

void APaperCharacterMage::GenerateAttackBullet()
{
	if (!bIsAttacking) return;
	ABulletMageAttackActor* BulletActor = GetWorld()->SpawnActor<ABulletMageAttackActor>(ABulletMageAttackActor::StaticClass(), BulletSocket->GetComponentLocation(), GetSprite()->GetRelativeRotation());
	if (BulletActor)
	{
		BulletActor->BulletFlyDirection = GetSprite()->GetForwardVector();
		BulletActor->Creator = this;
	}
}

void APaperCharacterMage::GenerateSkillBullet()
{
	if (!bIsSkilling) return;
	ABulletMageSkillActor* BulletActor = GetWorld()->SpawnActor<ABulletMageSkillActor>(ABulletMageSkillActor::StaticClass(), BulletSocket->GetComponentLocation(), GetSprite()->GetRelativeRotation());
	if (BulletActor)
	{
		BulletActor->BulletFlyDirection = GetSprite()->GetForwardVector();
		BulletActor->Creator = this;
	}
}

