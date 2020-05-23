// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialBody.h"

// Sets default values
ACelestialBody::ACelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();

	Velocity = InitialVelocity;
}

// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FQuat rotation = FQuat(1.0, 1.0, 1.0, 1.0);
	RootComponent->MoveComponent(Velocity, rotation, false);

}

