// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActor.h"

#include "Components/SphereComponent.h"

// Sets default values
ASPowerupActor::ASPowerupActor() :
	RespawnTime(10.0f)
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
	SphereComp->SetCollisionProfileName("Powerup");
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes
}

void ASPowerupActor::ShowPowerup()
{
	SetPowerupState(true);
}

void ASPowerupActor::HideAndCooldownPowerup()
{
	SetPowerupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ThisClass::ShowPowerup, RespawnTime);
}

void ASPowerupActor::SetPowerupState(bool bNewIsActive)
{
	SetActorEnableCollision(bNewIsActive);

	// Set visibility on root and all children
	RootComponent->SetVisibility(bNewIsActive, true);
}
