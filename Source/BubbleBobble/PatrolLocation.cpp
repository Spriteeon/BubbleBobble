// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolLocation.h"
#include "Engine/Engine.h"

EBTNodeResult::Type UPatrolLocation::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	if (GEngine) /** Global engine pointer. Can be 0 so don't use without checking. */
	{
		GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::White, "PATROL C++");
	}

	return EBTNodeResult::Type();
}

EBTNodeResult::Type UPatrolLocation::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	return EBTNodeResult::Type();
}

FName UPatrolLocation::GetNodeIconName() const
{
	return FName();
}

void UPatrolLocation::OnGameplayTaskActivated(UGameplayTask & Task)
{
}

void UPatrolLocation::OnGameplayTaskDeactivated(UGameplayTask & Task)
{
}
