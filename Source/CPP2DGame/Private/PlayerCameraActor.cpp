// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraActor.h"
#include "PaperCharacterBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCameraActor::APlayerCameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));

	ArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("ArmComp"));
	ArmComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	ArmComp->bDoCollisionTest = false;
	ArmComp->SetupAttachment(RootComponent);
	
	MainCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	MainCameraComp->SetProjectionMode(ECameraProjectionMode::Orthographic);
	MainCameraComp->SetOrthoWidth(1304.f);
	MainCameraComp->SetupAttachment(ArmComp);

	CurrentPlayerCharacter = nullptr;
}

// Called when the game starts or when spawned
void APlayerCameraActor::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		PC->SetViewTarget(this);
		CurrentPlayerCharacter = Cast<APaperCharacterBase>(PC->GetCharacter());
	}
}

// Called every frame
void APlayerCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FollowingCharacter();
}

void APlayerCameraActor::FollowingCharacter()
{
	if (CurrentPlayerCharacter)
	{
		FVector TempLocation = CurrentPlayerCharacter->GetActorLocation();
		if (TempLocation.X < LeftLocationX || TempLocation.X > RightLocationX)
		{
			//SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, TempLocation.Z));
		}
		else
		{
			SetActorLocation(FVector(TempLocation.X, TempLocation.Y, GetActorLocation().Z));
			//SetActorLocation(TempLocation);
		}
	}
}

