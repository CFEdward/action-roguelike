// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerupActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this actor's properties
	ASPowerupActor();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
protected:

	UFUNCTION()
	void ShowPowerup();

	void HideAndCooldownPowerup();

	void SetPowerupState(bool bNewIsActive);

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, Category = "Powerup")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

};
