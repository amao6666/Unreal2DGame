// Copyright Epic Games, Inc. All Rights Reserved.


#include "CPP2DGameGameModeBase.h"

#include "GameHUD.h"
#include "PaperCharacterMage.h"

ACPP2DGameGameModeBase::ACPP2DGameGameModeBase()
{
	DefaultPawnClass = LoadClass<APaperCharacterMage>(this, TEXT("Blueprint'/Game/Characters/BP_CharacterMage.BP_CharacterMage_c'"));
	HUDClass = AGameHUD::StaticClass();
}
