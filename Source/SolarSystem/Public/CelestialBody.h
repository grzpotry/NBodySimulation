// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KinematicBody.h"
#include "GameFramework/Actor.h"
#include "CelestialBody.generated.h"


UCLASS()
class SOLARSYSTEM_API ACelestialBody : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Physics)
	FVector InitialVelocity;

	//just proof of concept - calculate initial params for moon orbit
	UPROPERTY(EditAnywhere, Category = Physics)
    bool CalculateInitialVelocityAndPosition;

	UPROPERTY(EditAnywhere, Category = Physics)
	float InitialVelocityMagnitude;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = Physics)
	float Mass;

	UPROPERTY(VisibleAnywhere, Category = Physics)
	FVector Velocity;

	UPROPERTY(EditAnywhere, Category = Physics)
	float DebugArrowLength = 20;

	UPROPERTY(BlueprintReadOnly)
	TArray<FVector> PredictedTrajectory;

	FKinematicBody GetKinematic() const;

	void DrawDebugVelocityVector() const;
	void DrawDebugForces(TArray<ACelestialBody*> allBodies, float GravityConst);

	void UpdatePosition(float DeltaTime);

	// Sets default values for this actor's properties
	ACelestialBody();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
