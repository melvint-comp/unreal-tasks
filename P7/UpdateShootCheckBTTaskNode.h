// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UpdateShootCheckBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class P6_USERINTERFACES_API UUpdateShootCheckBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};