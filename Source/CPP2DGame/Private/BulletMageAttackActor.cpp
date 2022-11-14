// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletMageAttackActor.h"
#include "PaperFlipbook.h"

ABulletMageAttackActor::ABulletMageAttackActor()
{
	BulletFlyFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Animations/Bullet/FB_MageAttackFly.FB_MageAttackFly'"));
	BulletHitFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Animations/Bullet/FB_MageAttackHit.FB_MageAttackHit'"));
	BoxCollision->SetBoxExtent(FVector(4.f, 50.f, 4.f));
	BoxCollision->SetRelativeLocation(FVector(10.f, 0.f, -10.f));
}
