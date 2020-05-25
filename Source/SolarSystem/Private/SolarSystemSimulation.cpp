// Fill out your copyright notice in the Description page of Project Settings.


#include "SolarSystemSimulation.h"
#include "Kismet/GameplayStatics.h"
#include "CelestialBody.h"
#include "DrawDebugHelpers.h"


// Called every frame
void ASolarSystemSimulation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<FKinematicBody> kinematicBodies;

	for (ACelestialBody* body : Bodies)
	{
		FKinematicBody b = body->GetKinematic();
		kinematicBodies.Add(b);
	}

	for (ACelestialBody* body : Bodies)
	{
		body->Velocity = body->GetKinematic().CalculateVelocity(kinematicBodies, Gravity, DeltaTime);
		//body->DrawDebugForces(Bodies, Gravity);
	}

	for (ACelestialBody* body : Bodies)
	{
		body->UpdatePosition();
	}

	float sample = 0.01;
	float simulationSec = 10;

	// FVector startPos = kinematicBodies[0]->Position;
	// GLog->Log("BEFORE SIMULATION "  + kinematicBodies[0]->Position.ToString());

	for(float i = 0; i < simulationSec; i+=sample)
	{
		for (int index =0; index < kinematicBodies.Num(); index ++)
		{
			FVector newVelocity = kinematicBodies[index].CalculateVelocity(kinematicBodies, Gravity, sample);
			FVector newPosition = kinematicBodies[index].Position + newVelocity;

			kinematicBodies[index].Position = newPosition;
			kinematicBodies[index].Velocity = newVelocity;

			//TODO: optional args?


			DrawDebugPoint(GetWorld(), newPosition, 1000, FColor::Red, false);
			//GLog->Log(kinematicBodies[index].Position.ToString());
		}
	}

	// DrawDebugDirectionalArrow(GetWorld(), startPos, kinematicBodies[0]->Position, 100, FColor::Blue);
	// GLog->Log("AFTER SIMULATION " + kinematicBodies[0]->Position.ToString());
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
