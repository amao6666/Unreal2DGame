// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperCharacterEnemyBase.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APaperCharacterEnemyBase::APaperCharacterEnemyBase()
{
	Damage = 10.f;
	MaxHP = 100.f;
	HP = 100.f;
	WalkSpeed = 200.f;
	ToIdle = FEnemyMovementChanger({EEnemyMovementState::Idle, true, &APaperCharacterEnemyBase::GetIsStop});
	ToWalk = FEnemyMovementChanger({EEnemyMovementState::Walk, true, &APaperCharacterEnemyBase::GetIsWalking});
	ToAttack = FEnemyMovementChanger({EEnemyMovementState::Attack, false, &APaperCharacterEnemyBase::GetIsAttacking});
	ToHit = FEnemyMovementChanger({EEnemyMovementState::Hurt, false, &APaperCharacterEnemyBase::GetIsHiting});
	ToDeath = FEnemyMovementChanger({EEnemyMovementState::Death, false, &APaperCharacterEnemyBase::GetIsDead});

	TArray<FEnemyMovementChanger> IdleArray;
	IdleArray.Add(ToWalk);
	IdleArray.Add(ToAttack);
	IdleArray.Add(ToHit);
	IdleArray.Add(ToDeath);
    MovementStateDataMap.Add(EEnemyMovementState::Idle, IdleArray);

	TArray<FEnemyMovementChanger> WalkArray;
	WalkArray.Add(ToIdle);
	WalkArray.Add(ToAttack);
	WalkArray.Add(ToHit);
	WalkArray.Add(ToDeath);
	MovementStateDataMap.Add(EEnemyMovementState::Walk, WalkArray);
	
	TArray<FEnemyMovementChanger> AttackArray;
	AttackArray.Add(ToIdle);
	AttackArray.Add(ToWalk);
	AttackArray.Add(ToAttack);
	AttackArray.Add(ToDeath);
	MovementStateDataMap.Add(EEnemyMovementState::Attack, AttackArray);

	TArray<FEnemyMovementChanger> HitArray;
	HitArray.Add(ToIdle);
	HitArray.Add(ToWalk);
	HitArray.Add(ToAttack);
	HitArray.Add(ToDeath);
	MovementStateDataMap.Add(EEnemyMovementState::Hurt, HitArray);
}

void APaperCharacterEnemyBase::OnSpriteFinshPlaying()
{
	FString AnimaName = GetSprite()->GetFlipbook()->GetName();
	if (AnimaName.Contains(TEXT("Attack")))
	{
		OnAttackEnd();
	}
	else if (AnimaName.Contains(TEXT("Hurt")))
	{
		OnHitEnd();
	}
	else if (AnimaName.Contains(TEXT("Death")))
	{
		Destroy();
	}
}

void APaperCharacterEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &APaperCharacterEnemyBase::OnSpriteFinshPlaying);
}

void APaperCharacterEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (MovementStateDataMap.Contains(CurrentEnemyMovementState))
	{
		for (auto Item : MovementStateDataMap[CurrentEnemyMovementState])
		{
			if ((this->*Item.ConditionFunc)())
			{
				ChangeEnemyMovementState(Item.TargetState, Item.bIsLoop);
				break;
			}
		}
	}
}

void APaperCharacterEnemyBase::ChangeEnemyMovementState(EEnemyMovementState NewAnimation, bool bIsLoop)
{
	CurrentEnemyMovementState = NewAnimation;
	if (EnemyAnimationMap.Contains(NewAnimation))
	{
		UPaperFlipbookComponent* SpritePtr = GetSprite();
		GetSprite()->SetFlipbook(EnemyAnimationMap[NewAnimation]);
		SpritePtr->SetLooping(bIsLoop);
		SpritePtr->Play();
	}
}

void APaperCharacterEnemyBase::OnAttack()
{
	if (bIsHiting) bIsHiting = false;
	bIsAttacking = true;
}

void APaperCharacterEnemyBase::OnAttackEnd()
{
	bIsAttacking = false;
}

void APaperCharacterEnemyBase::OnHit(float DamageValue)
{
	HP -= DamageValue;
	if (HP <= 0.f)
	{
		OnDead();
		return;
	}
	if (!bIsAttacking)
	{
		bIsHiting = true;
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void APaperCharacterEnemyBase::OnHitEnd()
{
	bIsHiting = false;
	GetCharacterMovement()->MaxWalkSpeed = 200;
}

void APaperCharacterEnemyBase::OnDead()
{
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	OnDead_Implement();
}

bool APaperCharacterEnemyBase::GetIsWalking()
{
	return !GetIsStop() \
		&& !bIsAttacking \
		&& !bIsHiting \
		&& GetCharacterMovement()->Velocity.Size2D() <= WalkSpeed;
}

bool APaperCharacterEnemyBase::GetIsStop()
{
	return GetVelocity().X == 0 \
		&& !bIsAttacking \
		&& !bIsHiting;
}

bool APaperCharacterEnemyBase::GetIsAttacking()
{
	return bIsAttacking;
}

bool APaperCharacterEnemyBase::GetIsHiting()
{
	return bIsHiting;
}

bool APaperCharacterEnemyBase::GetIsDead()
{
	return HP <= 0.f;
}
