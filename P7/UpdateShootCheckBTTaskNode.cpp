// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "UpdateShootCheckBTTaskNode.h"

EBTNodeResult::Type UUpdateShootCheckBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	UBehaviorTreeComponent* BTComp = &OwnerComp;
	if(!BTComp)
		return EBTNodeResult::Failed;
	AEnemyAIController* BTController = Cast<AEnemyAIController>(BTComp->GetOwner());
	if(!BTController)
		return EBTNodeResult::Failed;
	BTController->UpdateShootCheck();
	return EBTNodeResult::Succeeded;
}