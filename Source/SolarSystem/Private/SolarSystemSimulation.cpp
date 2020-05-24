// Fill out your copyright notice in the Description page of Project Settings.


#include "SolarSystemSimulation.h"
#include "Kismet/GameplayStatics.h"
#include "CelestialBody.h"


// Called every frame
void ASolarSystemSimulation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (ACelestialBody* body : Bodies)
	{
		body->UpdateVelocity(Bodies, Gravity, DeltaTime);
		body->UpdatePosition();
	}
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
