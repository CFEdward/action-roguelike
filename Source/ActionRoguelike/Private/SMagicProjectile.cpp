// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile() :
	DamageAmount(20.0f)
{
	SphereComp->SetSphereRadius(20.0f);
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnActorOverlap);
}

void ASMagicProjectile::OnActorOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		if (USAttributeComponent* AttributeComp =
				Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
			AttributeComp)
		{
			AttributeComp->ApplyHealthChange(GetInstigator(), -DamageAmount);

			// Only explode when we hit something valid
			Explode();
		}
	}
}
