// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "CelestialBody.h"
#include "NBodySimulation.h"

void ANBodySimulation::BeginPlay()
{
	Super::BeginPlay();

	Bodies.Empty();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACelestialBody::StaticClass(), actors);

	for (AActor* Actor : actors)
	{
		Bodies.Add(CastChecked<ACelestialBody>(Actor));
	}
}

void ANBodySimulation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<FKinematicBody> kinematicBodies;

	for (ACelestialBody* body : Bodies)
	{
		body->bDrawOrbit = bDrawOrbits;
		FKinematicBody b = body->GetKinematic();
		kinematicBodies.Add(b);
	}

	if (bSimulate)
	{
		for (ACelestialBody*const body : Bodies)
		{
			body->Velocity = body->GetKinematic().CalculateVelocity(MassMultiplier, kinematicBodies, Gravity,DeltaTime * SpeedFactor);
			body->DrawDebugForces(Bodies, Gravity);
		}

		for (ACelestialBody*const body : Bodies)
		{
			body->UpdatePosition(DeltaTime * SpeedFactor);
		}
	}

	UpdatePredictedOrbits(kinematicBodies);
}

void ANBodySimulation::UpdatePredictedOrbits(TArray<FKinematicBody>& kinematicBodies)
{
	int pointsCount = ceil(OrbitForecastLength / OrbitSamplingMultiplier);

	FVector centralBodyInitPosition;
	for (ACelestialBody* body : Bodies)
	{
		body->PredictedOrbit.SetNum(pointsCount + 1);
		body->PredictedOrbit[0] = body->GetRootComponent()->GetComponentLocation();

		if (body == CentralBody)
		{
			centralBodyInitPosition = body->GetKinematic().Position;
		}
	}

	int stepIndex = 1;
	for (float i = 0; i < OrbitForecastLength; i += OrbitSamplingMultiplier, stepIndex++)
	{
		UpdatePredictedOrbitsStep(kinematicBodies, centralBodyInitPosition, stepIndex);
	}
}

void ANBodySimulation::UpdatePredictedOrbitsStep(TArray<FKinematicBody>& kinematicBodies, FVector centralBodyInitPosition, int stepIndex)
{
	TArray<FVector> positions;
	TArray<FVector> velocities;

	positions.SetNum(kinematicBodies.Num());
	velocities.SetNum(kinematicBodies.Num());

	FVector centralBodyPositionDelta;

	//calc positions and velocities
	for (int bodyIndex = 0; bodyIndex < kinematicBodies.Num(); bodyIndex ++)
	{
		FVector newVelocity = kinematicBodies[bodyIndex].CalculateVelocity(MassMultiplier, kinematicBodies, Gravity, OrbitSamplingMultiplier);
		FVector newPosition = kinematicBodies[bodyIndex].Position + newVelocity * OrbitSamplingMultiplier;

		velocities[bodyIndex] = newVelocity;
		positions[bodyIndex] = newPosition;

		if (CentralBody != nullptr && Bodies[bodyIndex] == CentralBody)
		{
			centralBodyPositionDelta = newPosition - centralBodyInitPosition;
		}
	}

	//update positions and velocities
	for (int bodyIndex = 0; bodyIndex < kinematicBodies.Num(); bodyIndex ++)
	{
		FVector newPosition = positions[bodyIndex];

		//Update position of each body by CentralBody move offset - (so we can draw all orbits in relation to one particular body)
		if (CentralBody != nullptr)
		{
			if (Bodies[bodyIndex] != CentralBody)
			{
				newPosition -= centralBodyPositionDelta;
			}
			else
			{
				newPosition = centralBodyInitPosition;
			}
		}


		Bodies[bodyIndex]->PredictedOrbit[stepIndex] = newPosition;
		kinematicBodies[bodyIndex].Position = newPosition;
		kinematicBodies[bodyIndex].Velocity = velocities[bodyIndex];
	}
}
