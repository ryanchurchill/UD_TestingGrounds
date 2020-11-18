// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "UD_TestingGroundsGameMode.h"
#include "UD_TestingGroundsHUD.h"
#include "Player/FirstPersonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUD_TestingGroundsGameMode::AUD_TestingGroundsGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Dynamic/Character/Behavior/BP_Character"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AUD_TestingGroundsHUD::StaticClass();
}
