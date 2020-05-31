// Fill out your copyright notice in the Description page of Project Settings.


#include "SolarSystemSimulation.h"
#include "Kismet/GameplayStatics.h"
#include "CelestialBody.h"
#include "DrawDebugHelpers.h"

void ASolarSystemSimulation::DrawTrajectories(TArray<FKinematicBody> kinematicBodies) const
{
	float sample = 1 / ForecastSamplePrecisionMultiplier;

	int pointsCount  = ceil(PathForecastLength / sample);

	for (ACelestialBody* body : Bodies)
	{
		body->PredictedTrajectory.SetNum(pointsCount);
	}

	//TODO: clean this up
	int pointIndex = 0;
	for(float i = 0; i < PathForecastLength; i+=sample, pointIndex++)
	{
		for (int bodyIndex =0; bodyIndex < kinematicBodies.Num(); bodyIndex ++)
		{
			FVector newVelocity = kinematicBodies[bodyIndex].CalculateVelocity(kinematicBodies, Gravity, sample);
			FVector newPosition = kinematicBodies[bodyIndex].Position + newVelocity * sample;

			Bodies[bodyIndex]->PredictedTrajectory[pointIndex] = newPosition;

			kinematicBodies[bodyIndex].Position = newPosition;
			kinematicBodies[bodyIndex].Velocity = newVelocity;

			//DrawDebugPoint(GetWorld(), newPosition, 1000, FColor::Red, false);
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
			body->Velocity = body->GetKinematic().CalculateVelocity(kinematicBodies, Gravity, DeltaTime);
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
