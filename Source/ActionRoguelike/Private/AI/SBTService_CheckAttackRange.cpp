// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// Check distance between AI Pawn and TargetActor
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent(); ensure(BlackboardComp))
	{
		if (const AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor")))
		{
			if (const AAIController* MyController = OwnerComp.GetAIOwner(); ensure(MyController))
			{
				if (const APawn* AIPawn = MyController->GetPawn(); ensure(AIPawn))
				{
					const float DistanceTo = FVector::Distance(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					const bool bWithinRange = DistanceTo < 2'000.0f;

					bool bHasLineOfSight = false;
					if (bWithinRange)
					{
						bHasLineOfSight = MyController->LineOfSightTo(TargetActor);
					}
					
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bHasLineOfSight);
				}
			}
		}
	}
}
