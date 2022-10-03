// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent() :
	Health(100.0f),
	MaxHealth(Health)
{
	
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, const float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}
	
	const float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);

	const float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	return ActualDelta != 0;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const
{
	return Health == MaxHealth;
}

float USAttributeComponent::GetHealthMax() const
{
	return MaxHealth;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	if (const USAttributeComponent* AttributeComp = GetAttributes(Actor))
	{
		return AttributeComp->IsAlive();
	}

	return false;
}
