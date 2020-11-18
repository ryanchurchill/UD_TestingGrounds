// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "AI/Navigation/NavMeshBoundsVolume.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Array.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// TODO: all we should really expose to designer is size
	MinExtent = FVector(0, -2000, 0);
	MaxExtent = FVector(4000, 2000, 0);
	NavigationBoundsOffset = FVector(2000, 0, 0);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogTemp, Warning, TEXT("[%s] EndPlay"), *GetName());
	Pool->Return(MyNavMeshBoundsVolume);
}

void ATile::SetPool(UActorPool * InPool)
{
	Pool = InPool;

	// TODO: Side effect! Bad pattern?
	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	// TODO: bad pattern
	MyNavMeshBoundsVolume = Cast<ANavMeshBoundsVolume>(Pool->Checkout());
	// if fails, there's probably not enough actors in pool
	if (ensure(MyNavMeshBoundsVolume)) {
		UE_LOG(LogTemp, Warning, TEXT("[%s] Moving MyNavMeshBoundsVolume"), *GetName());
		MyNavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
		GetWorld()->GetNavigationSystem()->Build();
	}
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		if (FindEmptySpawnPosition(SpawnPosition, MinSpawn, MaxSpawn, Radius, MinScale, MaxScale))
		{
			PlaceActor(ToSpawn, SpawnPosition);
		}
	}
}

// TODO: lots of duplicate code with method above
void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius)
{
	int NumberToSpawn = FMath::RandRange(MinSpawn, MaxSpawn);
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		FSpawnPosition SpawnPosition;
		if (FindEmptySpawnPosition(SpawnPosition, MinSpawn, MaxSpawn, Radius, 1, 1))
		{
			APawn* Enemy = Cast<APawn>(PlaceActor(ToSpawn, SpawnPosition));
			Enemy->SpawnDefaultController();
			// TODO: magic string
			Enemy->Tags.Add("Guard");
		}
	}
}

// huge bug: spawn positions may not be empty any more
bool ATile::FindEmptySpawnPosition(FSpawnPosition &outSpawnPosition, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	outSpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
	if (FindEmptyLocation(outSpawnPosition.Location, Radius * outSpawnPosition.Scale)) {
		outSpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);
		return true;
	}
	return false;
}

bool ATile::FindEmptyLocation(FVector& OutLocation, float Radius)
{
	FBox Bounds = FBox(MinExtent, MaxExtent);

	const int MAX_ATTEMPTS = 100;
	for (size_t i = 0; i < MAX_ATTEMPTS; i++) {
		FVector CandidatePoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidatePoint, Radius)) {
			OutLocation = CandidatePoint;
			return true;
		}
	}
	
	return false;
}

AActor* ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(
		ToSpawn, SpawnPosition.Location, FRotator(0, SpawnPosition.Rotation, 0));
	// Udemy SetActorRelativeLocation instead of passing SpawnPoint above
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
	return Spawned;
}



// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// uses Sweep to do a SphereCast to tell us if there's an intersection
bool ATile::CanSpawnAtLocation(FVector LocalLocation, float Radius)
{
	FVector GlobalLocation = ActorToWorld().TransformPosition(LocalLocation);

	FHitResult HitResult;
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	// debug
	//FColor ResultColor = HasHit ? FColor::Red : FColor::Green;
	//DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);

	return !HasHit;
}
