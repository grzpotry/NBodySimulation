// Fill out your copyright notice in the Description page of Project Settings.

#include "SolarSystemSimulation.h"
#include "Kismet/GameplayStatics.h"
#include "CelestialBody.h"

void ASolarSystemSimulation::DrawTrajectories(TArray<FKinematicBody> kinematicBodies) const
{

	int pointsCount  = ceil(PathForecastLength / TrajectorySamplingMultiplier);
	TArray<FVector> initialPositions;
	initialPositions.SetNum(kinematicBodies.Num());

	for (ACelestialBody* body : Bodies)
	{
		body->PredictedTrajectory.SetNum(pointsCount + 1);
		body->PredictedTrajectory[0] = body->GetRootComponent()->GetComponentLocation();
	}

	//TODO: clean this up
	int pointIndex = 1;
	for(float i = 0; i < PathForecastLength; i+=TrajectorySamplingMultiplier, pointIndex++)
	{
		TArray<FVector> positions;
		TArray<FVector> velocities;

		positions.SetNum(kinematicBodies.Num());
		velocities.SetNum(kinematicBodies.Num());

		//calc positions
		for (int bodyIndex =0; bodyIndex < kinematicBodies.Num(); bodyIndex ++)
		{
			FVector newVelocity = kinematicBodies[bodyIndex].CalculateVelocity(MassMultiplier, kinematicBodies, Gravity, TrajectorySamplingMultiplier);
			FVector newPosition = kinematicBodies[bodyIndex].Position + newVelocity * TrajectorySamplingMultiplier;

			velocities[bodyIndex] = newVelocity;
			positions[bodyIndex] = newPosition;
		}


		for (int bodyIndex =0; bodyIndex < kinematicBodies.Num(); bodyIndex ++)
		{
			//eg. earth for moon
			ACelestialBody * centralBody = Bodies[bodyIndex]->CentralBody;
			FVector newPosition = positions[bodyIndex];

			// TODO: Fix calculating trajectory relative to central body
			// if (centralBody != nullptr && pointIndex > 0)
			// {
			// 	int centralBodyIndex = Bodies.Find(centralBody);
			// 	FVector centralBodyMoveDeltaMove = Bodies[centralBodyIndex]->PredictedTrajectory[0] - Bodies[bodyIndex]->PredictedTrajectory[0];//Bodies[centralBodyIndex]->PredictedTrajectory[pointIndex - 1];
			// 	newPosition -= centralBodyMoveDeltaMove;
			// }

			Bodies[bodyIndex]->PredictedTrajectory[pointIndex] = newPosition;
			kinematicBodies[bodyIndex].Position = newPosition;
			kinematicBodies[bodyIndex].Velocity = velocities[bodyIndex];
		}
	}
}

void ASolarSystemSimulation::Tick(float DeltaTime)
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

void ASolarSystemSimulation::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACelestialBody::StaticClass(), actors);

	for (AActor * Actor : actors)
	{
		Bodies.Add(CastChecked<ACelestialBody>(Actor));
	}
}
