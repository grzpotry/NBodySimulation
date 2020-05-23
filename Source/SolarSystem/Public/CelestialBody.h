// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CelestialBody.generated.h"

UCLASS()
class SOLARSYSTEM_API ACelestialBody : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Physics)
	FVector InitialVelocity;

	UPROPERTY(VisibleAnywhere, Category = Physics)
    FVector Velocity;

	// Sets default values for this actor's properties
	ACelestialBody();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
