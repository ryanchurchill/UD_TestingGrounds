// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

class UActorPool;

UCLASS()
class UD_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	// must be called by BP when we spawn the tile. Bad Pattern?
	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* InPool);

	UFUNCTION(BlueprintCallable)
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 3, float Radius = 500, float MinScale = 1, float MaxScale = 1);

	UFUNCTION(BlueprintCallable)
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn = 1, int MaxSpawn = 3, float Radius = 500);


	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

private:
	UActorPool* Pool;
	class ANavMeshBoundsVolume* MyNavMeshBoundsVolume;

	void PositionNavMeshBoundsVolume();
	
	bool CanSpawnAtLocation(FVector LocalLocation, float Radius);
	
	bool FindEmptyLocation(FVector& OutLocation, float Radius);

	bool ATile::FindEmptySpawnPosition(FSpawnPosition &outSpawnPosition, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale);

	AActor* PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);
};
