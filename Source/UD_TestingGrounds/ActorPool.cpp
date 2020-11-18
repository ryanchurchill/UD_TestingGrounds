// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"
#include "Queue.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

AActor * UActorPool::Checkout()
{
	if (!Pool.IsEmpty()) {
		AActor* Ret;
		Pool.Dequeue(Ret);
		UE_LOG(LogTemp, Warning, TEXT("Checking Out: %s"), *Ret->GetName());
		return Ret;
	}
	UE_LOG(LogTemp, Warning, TEXT("No Actor to checkout"));
	return nullptr;
}

void UActorPool::Return(AActor * ActorToReturn)
{
	Add(ActorToReturn);
}

void UActorPool::Add(AActor * ActorToAdd)
{
	if (ensure(ActorToAdd)) {
		UE_LOG(LogTemp, Warning, TEXT("Adding to Pool: %s"), *ActorToAdd->GetName());
		Pool.Enqueue(ActorToAdd);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("No Actor to add"));
	}
}
