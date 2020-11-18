// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UD_TestingGroundsHUD.generated.h"

UCLASS()
class AUD_TestingGroundsHUD : public AHUD
{
	GENERATED_BODY()

public:
	AUD_TestingGroundsHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

