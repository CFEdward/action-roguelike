// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Perception/PawnSensingComponent.h"

ASAICharacter::ASAICharacter() :
	TimeToHitParamName("TimeToHit")
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ThisClass::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void ASAICharacter::SetTargetActor(AActor* NewTarget) const
{
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	
	DrawDebugString(
    	GetWorld(),
    	GetActorLocation(),
    	"PLAYER SPOTTED",
    	nullptr,
    	FColor::White,
    	4.0f,
    	true
    );
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
            if (ActiveHealthBar)
            {
            	ActiveHealthBar->AttachedActor = this;
            	ActiveHealthBar->AddToViewport();
            }
		}
		
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);
		
		if (NewHealth <= 0.0f)
		{
			// stop BT
			if (const AAIController* AIC = Cast<AAIController>(GetController()))
			{
				AIC->GetBrainComponent()->StopLogic("Killed");
			}
			// ragdoll
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetMesh()->SetCollisionProfileName("Ragdoll");
			// Set lifespan
			SetLifeSpan(10.0f);
		}
	}
}
