// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "CelestialBody.h"
#include "NBodySimulation.h"

void ANBodySimulation::DrawTrajectories(TArray<FKinematicBody> kinematicBodies) const
{

	int pointsCount  = ceil(PathForecastLength / TrajectorySamplingMultiplier);



	FVector centralBodyInitPosition;
	for (ACelestialBody* body : Bodies)
	{
		body->PredictedTrajectory.SetNum(pointsCount + 1);
		body->PredictedTrajectory[0] = body->GetRootComponent()->GetComponentLocation();

		if (body == CentralBody)
		{
			centralBodyInitPosition = body->GetKinematic().Position;
		}
	}



	//simulation
	int pointIndex = 1;
	for(float i = 0; i < PathForecastLength; i+=TrajectorySamplingMultiplier, pointIndex++)
	{
		TArray<FVector> positions;
		TArray<FVector> velocities;

		positions.SetNum(kinematicBodies.Num());
		velocities.SetNum(kinematicBodies.Num());

		FVector centralBodyPositionDelta;

		//calc positions and velocities
		for (int bodyIndex =0; bodyIndex < kinematicBodies.Num(); bodyIndex ++)
		{
			FVector newVelocity = kinematicBodies[bodyIndex].CalculateVelocity(MassMultiplier, kinematicBodies, Gravity, TrajectorySamplingMultiplier);
			FVector newPosition = kinematicBodies[bodyIndex].Position + newVelocity * TrajectorySamplingMultiplier;

			velocities[bodyIndex] = newVelocity;
			positions[bodyIndex] = newPosition;

			if (CentralBody != nullptr && Bodies[bodyIndex] == CentralBody)
			{
				centralBodyPositionDelta = newPosition - centralBodyInitPosition;
			}
		}


		for (int bodyIndex =0; bodyIndex < kinematicBodies.Num(); bodyIndex ++)
		{
			FVector newPosition = positions[bodyIndex];

			//przesuwamy kazde cialo o wartosc przesuniecia ciala centralnego
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

			Bodies[bodyIndex]->PredictedTrajectory[pointIndex] = newPosition;
			kinematicBodies[bodyIndex].Position = newPosition;
			kinematicBodies[bodyIndex].Velocity = velocities[bodyIndex];
		}
	}
}

void ANBodySimulation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<FKinematicBody> kinematicBodies;

	for (ACelestialBody* body : Bodies)
	{
		FKinematicBody b = body->GetKinematic();
		kinematicBodies.Add(b);
	}

	if (bSimulate)
	{
		for (ACelestialBody* body : Bodies)
		{
			body->Velocity = body->GetKinematic().CalculateVelocity(MassMultiplier, kinematicBodies, Gravity, DeltaTime);
			body->DrawDebugForces(Bodies, Gravity);
		}

		for (ACelestialBody* body : Bodies)
		{
			body->UpdatePosition(DeltaTime);
		}
	}

	DrawTrajectories(kinematicBodies);
}

void ANBodySimulation::BeginPlay()
{
	Super::BeginPlay();

	Bodies.Empty();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACelestialBody::StaticClass(), actors);

	for (AActor * Actor : actors)
	{
		Bodies.Add(CastChecked<ACelestialBody>(Actor));
	}
}
