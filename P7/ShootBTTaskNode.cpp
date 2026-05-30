// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "ShootBTTaskNode.h"

EBTNodeResult::Type UShootBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* BTComp = &OwnerComp;
	if(!BTComp)
		return EBTNodeResult::Failed;
	AEnemyAIController* BTController =
	 Cast<AEnemyAIController>(BTComp->GetOwner());
	if(!BTController)
		return EBTNodeResult::Failed;
	BTController->Shoot();
	return EBTNodeResult::Succeeded;
}