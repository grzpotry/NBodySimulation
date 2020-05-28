// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CelestialBody.generated.h"

USTRUCT()
struct FKinematicBody
{
	GENERATED_USTRUCT_BODY()

	FVector Velocity;
	FVector Position;
	float Mass;

	FKinematicBody(): Mass(0)
	{
	}

	FKinematicBody(const FVector& position_, const FVector& velocity_, const float mass_): Mass(mass_), Velocity(velocity_), Position(position_)
	{
	}


	FVector CalculateVelocity(TArray<FKinematicBody> otherBodies, float GravityConst, float DeltaTime) const
	{
		FVector newVelocity = Velocity;

		for (FKinematicBody other : otherBodies)
		{
			//F = G(m1m2)/r^2 * e
			FVector x = other.Position - Position;

			if (x.Equals(FVector::ZeroVector))
			{
				//skip self
				continue;
			}

			FVector forceDir = x.GetSafeNormal();
			FVector force = GravityConst * (Mass * other.Mass) / x.SizeSquared() * forceDir;

			//F = ma
			FVector acceleration = force / Mass;
			newVelocity += acceleration * DeltaTime;
		}

		return newVelocity;
	}
};


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

	UPROPERTY(VisibleAnywhere, Category = Physics)
	FVector Velocity;

	UPROPERTY(EditAnywhere, Category = Physics)
	float DebugArrowLength = 20;

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
