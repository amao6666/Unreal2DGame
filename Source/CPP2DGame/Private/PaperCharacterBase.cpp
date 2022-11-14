// Fill out your copyright notice in the Description page of Project Settings.


#include "PaperCharacterBase.h"
#include "ClimbActor.h"
#include "StaticItemActor.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

APaperCharacterBase::APaperCharacterBase()
{
	ToIdle = FMovementStateChanger({EHeroMovementState::Idle, true, &APaperCharacterBase::GetIsStop});
	ToWalk = FMovementStateChanger({EHeroMovementState::Walk, true, &APaperCharacterBase::GetIsWalking});
	ToRun = FMovementStateChanger({EHeroMovementState::Run, true, &APaperCharacterBase::GetIsRuning});
	ToAttack = FMovementStateChanger({EHeroMovementState::Attack, false, &APaperCharacterBase::GetIsAttacking});
	ToHit = FMovementStateChanger({EHeroMovementState::Hurt, false, &APaperCharacterBase::GetIsHiting});
	ToSkill = FMovementStateChanger({EHeroMovementState::Skill, false, &APaperCharacterBase::GetIsSkilling});
	ToJumpUp = FMovementStateChanger({EHeroMovementState::Jump_Up, false, &APaperCharacterBase::GetIsFallingUp});
	ToJumpDown = FMovementStateChanger({EHeroMovementState::Jump_Down, false, &APaperCharacterBase::GetIsFallingDown});
	ToJumpLand = FMovementStateChanger({EHeroMovementState::Jump_Land, false, &APaperCharacterBase::GetIsLand});

	ToHighJumpUp = FMovementStateChanger({EHeroMovementState::High_Jump_Up, false, &APaperCharacterBase::GetIsHighJumpUp});
	ToHighJumpDown = FMovementStateChanger({EHeroMovementState::High_Jump_Down, false, &APaperCharacterBase::GetIsFallingDown});
	
	ToWalkAttack = FMovementStateChanger({EHeroMovementState::Walk_Attack, false, &APaperCharacterBase::GetIsAttacking});
	ToRunAttack = FMovementStateChanger({EHeroMovementState::Run_Attack, false, &APaperCharacterBase::GetIsAttacking});

	ToClimb = FMovementStateChanger({EHeroMovementState::Climb, true, &APaperCharacterBase::GetIsClimbing});
	ToClimbIdle = FMovementStateChanger({EHeroMovementState::Climb_Idle, false, &APaperCharacterBase::GetIsClimbIdle});
	
	CurrentJumpLevel = 0;
	CoinNumber = 0;
	
	bIsAttacking = false;
	bIsSkilling = false;
	bCanClimb = false;
	bIsClimbing = false;
	bIsInClimb = false;
	bIsOnClimbTop = false;
	bIsHiting = false;

	CanClimbActor = nullptr;
}

void APaperCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	GetSprite()->OnFinishedPlaying.AddDynamic(this, &APaperCharacterBase::OnSpriteFinshPlaying);
}

void APaperCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Movement"), this, &APaperCharacterBase::OnMove);
	PlayerInputComponent->BindAxis(TEXT("Climb"), this, &APaperCharacterBase::OnClimb);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APaperCharacterBase::OnJump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &APaperCharacterBase::OnAttack);
	PlayerInputComponent->BindAction(TEXT("Skill"), EInputEvent::IE_Pressed, this, &APaperCharacterBase::OnSkill);
	PlayerInputComponent->BindAction(TEXT("SpeedUp"), IE_Pressed, this, &APaperCharacterBase::OnSpeedUp);
	PlayerInputComponent->BindAction(TEXT("SpeedUp"), IE_Released, this, &APaperCharacterBase::OnSpeedDown);
}

void APaperCharacterBase::OnMove(float AxisValue)
{
	MovementStateMochini(AxisValue);
	if ((CurrentHeroMovementState == EHeroMovementState::Jump_Land || \
		CurrentHeroMovementState == EHeroMovementState::High_Jump_Down) \
		&& !GetIsFalling()) CurrentJumpLevel = 0;
	if (bIsInClimb) return;
	if (bIsSkilling) return;
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APaperCharacterBase::OnClimb(float AxisValue)
{
	if (!bCanClimb) return;
	if (!bIsInClimb && bIsOnClimbTop && AxisValue > 0) return;
	if (AxisValue != 0)
	{
		bIsInClimb = true;
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		if (CanClimbActor)
		{
			FVector TempLocation = GetActorLocation();
			TempLocation.X = CanClimbActor->GetActorLocation().X;
			SetActorLocation(TempLocation);
		}
	}
	bIsClimbing = (AxisValue != 0);
	AddMovementInput(GetActorUpVector(), AxisValue);
}

void APaperCharacterBase::OnJump()
{
	if (GetIsAttacking()) bIsAttacking = false;
	if (GetIsSkilling()) bIsSkilling = false;
	if (GetIsFalling() && CurrentJumpLevel == 0) return;
	if (GetIsClimbing() || GetIsHiting()) return;
	if (CurrentJumpLevel < 2) CurrentJumpLevel += 1;
	Jump();
}

void APaperCharacterBase::OnAttack()
{
}

void APaperCharacterBase::OnSkill()
{
}

void APaperCharacterBase::OnHit(float DamageValue)
{
	HP -= DamageValue;
	if (HP <= 0.f)
	{
		HP = 0.f;
		OnDead();
		return;
	}
	if (!bIsAttacking && !GetIsClimbing() && !GetIsFalling() && !GetIsSkilling())
	{
		bIsHiting = true;
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void APaperCharacterBase::OnHitEnd()
{
	bIsHiting = false;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APaperCharacterBase::OnDead()
{
	CurrentHeroMovementState = EHeroMovementState::Death;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;
	if (HeroAnimationMap.Contains(EHeroMovementState::Death))
	{
		GetSprite()->SetFlipbook(HeroAnimationMap[EHeroMovementState::Death]);
		GetSprite()->SetLooping(false);
		GetSprite()->Play();
	}
}

void APaperCharacterBase::OnDeadEnd()
{
	GetSprite()->SetHiddenInGame(true);
}

void APaperCharacterBase::OnSpeedUp()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void APaperCharacterBase::OnSpeedDown()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APaperCharacterBase::MovementStateMochini(float AxisValue)
{
	if (MovementStateMochiniDataMap.Contains(CurrentHeroMovementState))
	{
		for (auto Item : MovementStateMochiniDataMap[CurrentHeroMovementState])
		{
			if ((this->*Item.ConditionFunc)())
			{
				ChangeHeroMovementState(Item.TargetState, Item.bIsLoop);
				break;
			}
		}
	}
	if (AxisValue == 0 || GetIsFalling()) return;
	float YawValue = AxisValue > 0 ? 0.f : 180.f;
	GetSprite()->SetRelativeRotation(FRotator(0.f, YawValue, 0.f));
}

void APaperCharacterBase::ChangeHeroMovementState(EHeroMovementState NewAnimation, bool bIsLoop)
{
	CurrentHeroMovementState = NewAnimation;
	if (HeroAnimationMap.Contains(NewAnimation))
	{
		UPaperFlipbookComponent* SpritePtr = GetSprite();
		GetSprite()->SetFlipbook(HeroAnimationMap[NewAnimation]);
		SpritePtr->SetLooping(bIsLoop);
		SpritePtr->Play();
	}
}

void APaperCharacterBase::OnSpriteFinshPlaying()
{
	FString SpriteName = GetSprite()->GetFlipbook()->GetName();
	if (SpriteName.Contains(TEXT("Attack"))) bIsAttacking = false;
	if (SpriteName.Contains(TEXT("Skill"))) bIsSkilling = false;
	if (SpriteName.Contains(TEXT("Hurt"))) OnHitEnd();
	if (SpriteName.Contains(TEXT("Death"))) OnDeadEnd();
}

void APaperCharacterBase::OnBeginOverlapClimb(AClimbActor* ClimbActor)
{
	bCanClimb = true;
	CanClimbActor = ClimbActor;
}

void APaperCharacterBase::OnEndOverlapClimb(AClimbActor* ClimbActor)
{
	bCanClimb = false;
	if (CanClimbActor == ClimbActor) CanClimbActor = nullptr;
	if (bIsInClimb)
	{
		bIsClimbing = false;
		bIsInClimb = false;
		GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}
}

void APaperCharacterBase::OnBeginOverlapStaticItem(EStaticItemType ItemType)
{
	switch (ItemType)
	{
	case EStaticItemType::Coin:
		CoinNumber++;
		break;
	case EStaticItemType::Heart:
		HP = MaxHP;
		break;
	case EStaticItemType::BloodPag:
		HP += (MaxHP / 4);
		if (HP > MaxHP) HP = MaxHP;
		break;
	case EStaticItemType::Trap:
		OnHit(10.f);
		break;
	}
}

// is Condition of the change movement
bool APaperCharacterBase::GetIsFalling()
{
	return GetCharacterMovement()->IsFalling();
}

bool APaperCharacterBase::GetIsWalking()
{
	return !GetIsStop() \
		&& !GetIsFalling() \
		&& !bIsAttacking \
		&& !bIsSkilling \
		&& !bIsHiting \
		&& !GetIsClimbing() \
		&& GetCharacterMovement()->Velocity.Size2D() <= WalkSpeed;
}

bool APaperCharacterBase::GetIsRuning()
{
	return !GetIsStop() \
		&& !GetIsFalling() \
		&& !bIsAttacking \
		&& !bIsSkilling \
		&& !bIsHiting \
		&& !GetIsClimbing() \
		&& GetCharacterMovement()->Velocity.Size2D() > WalkSpeed + 10;
}

bool APaperCharacterBase::GetIsFallingUp()
{
	return GetIsFalling() \
		&& !bIsAttacking \
		&& !bIsSkilling \
		&& !bIsHiting \
		&& !GetIsClimbing() \
		&& GetVelocity().Z > 0;
}

bool APaperCharacterBase::GetIsFallingDown()
{
	return GetIsFalling() \
		&& !bIsAttacking \
		&& !bIsSkilling \
		&& !bIsHiting \
		&& !GetIsClimbing() \
		&& GetVelocity().Z < 0;
}

bool APaperCharacterBase::GetIsHighJumpUp()
{
	return CurrentJumpLevel == 2 \
		&& GetIsFallingUp();
}

bool APaperCharacterBase::GetIsLand()
{
	return !GetIsFalling() \
		&& !GetIsClimbing() \
		&& !bIsAttacking \
		&& !bIsHiting \
		&& !bIsSkilling;
}

bool APaperCharacterBase::GetIsStop()
{
	return GetVelocity().X == 0 \
		&& !GetIsFalling() \
		&& !GetIsClimbing() \
		&& !bIsAttacking \
		&& !bIsHiting \
		&& !bIsSkilling;
}

bool APaperCharacterBase::GetIsAttacking()
{
	return bIsAttacking;
}

bool APaperCharacterBase::GetIsSkilling()
{
	return bIsSkilling;
}

bool APaperCharacterBase::GetIsClimbing()
{
	return bCanClimb \
		&& bIsInClimb \
		&& bIsClimbing;
}

bool APaperCharacterBase::GetIsClimbIdle()
{
	return bCanClimb \
		&& bIsInClimb \
		&& !bIsClimbing;
}

bool APaperCharacterBase::GetIsHiting()
{
	return bIsHiting;
}

bool APaperCharacterBase::GetIsAnimPlayEnd()
{
	UPaperFlipbookComponent* SpritePtr = GetSprite();
	if(!SpritePtr->IsLooping())
	{
		return SpritePtr->GetPlaybackPosition() >= SpritePtr->GetFlipbookLength();
	}
	return false;
}
