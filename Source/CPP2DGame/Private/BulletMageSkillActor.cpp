// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletMageSkillActor.h"
#include "PaperFlipbook.h"

ABulletMageSkillActor::ABulletMageSkillActor()
{
	BulletFlyFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Animations/Bullet/FB_MageSkillFly.FB_MageSkillFly'"));
	BulletHitFlipbook = LoadObject<UPaperFlipbook>(this, TEXT("PaperFlipbook'/Game/Animations/Bullet/FB_MageSkillHit.FB_MageSkillHit'"));
	BoxCollision->SetBoxExtent(FVector(4.f, 50.f, 4.f));
	BoxCollision->SetRelativeLocation(FVector(10.f, 0.f, -10.f));
}
