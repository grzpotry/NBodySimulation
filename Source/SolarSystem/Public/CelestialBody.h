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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = Physics)
	float Mass;

	UPROPERTY(EditAnywhere, Category = Physics)
	float DebugArrowLength = 20;

	UPROPERTY(VisibleAnywhere, Category = Physics)
    FVector Velocity;

	void UpdateVelocity(TArray<ACelestialBody*> allBodies, float GravityConst, float DeltaTime);

	void UpdatePosition();

	// Sets default values for this actor's properties
	ACelestialBody();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
