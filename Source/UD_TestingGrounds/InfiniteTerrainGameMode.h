// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UD_TestingGroundsGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UD_TESTINGGROUNDS_API AInfiniteTerrainGameMode : public AUD_TestingGroundsGameMode
{
	GENERATED_BODY()
	
public:
	AInfiniteTerrainGameMode();

	void PopulateBoundsVolumePool();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pool")
	class UActorPool* NavMeshBoundsVolumePool;

	virtual void BeginPlay() override;

private:
	
	
	void AddToPool(class ANavMeshBoundsVolume *VolumeToAdd);
};
