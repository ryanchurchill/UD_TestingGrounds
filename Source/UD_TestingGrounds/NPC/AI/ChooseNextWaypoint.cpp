// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
//#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "NPC/AI/PatrolRoute.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		SetPatrolPoints(OwnerComp);
		if (PatrolPoints.Num() > 0) {
			int32 Index = SetNextWaypoint();
			CycleIndex(Index);
			//UE_LOG(LogTemp, Warning, TEXT("Waypoint index: %i"), Index);
		}
	}
	
	return EBTNodeResult::Succeeded;
}

void UChooseNextWaypoint::SetPatrolPoints(UBehaviorTreeComponent& OwnerComp)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto Owner = AIController->GetPawn();

	ACharacter* Guard = Cast<ACharacter>(Owner);
	if (!ensure(Guard)) return;

	UPatrolRoute* PatrolRoute = Guard->FindComponentByClass<UPatrolRoute>();
	if (!ensure(PatrolRoute)) return; // TODO: maybe this should fail (EBTNodeResult)

	PatrolPoints = PatrolRoute->GetPatrolPoints();
}

int32 UChooseNextWaypoint::SetNextWaypoint()
{
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);
	return Index;
}

void UChooseNextWaypoint::CycleIndex(int32 index)
{
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, (index + 1) % PatrolPoints.Num());
}
