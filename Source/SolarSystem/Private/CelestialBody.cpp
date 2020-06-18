// Fill out your copyright notice in the Description page of Project Settings.

#include "CelestialBody.h"
#include "DrawDebugHelpers.h"

FKinematicBody ACelestialBody::GetKinematic() const
{
	return FKinematicBody(RootComponent->GetComponentLocation(), Velocity, Mass);
}

void ACelestialBody::DrawDebugVelocityVector() const
{
	FVector pos = GetTransform().GetLocation();
	DrawDebugDirectionalArrow(GetWorld(),
	                          pos,
	                          pos + Velocity.GetSafeNormal() * DebugArrowLength,
	                          100, FColor::Green, false);
}

//TODO: draw simulated future orbit
void ACelestialBody::DrawDebugForces(const TArray<ACelestialBody*> allBodies, const float GravityConst)
{
	for (ACelestialBody *other : allBodies)
	{
		if (other == this)
		{
			continue;
		}

		FVector pos1 = GetTransform().GetLocation();
		FVector pos2 = other->GetRootComponent()->GetComponentLocation();

		//F = G(m1m2)/r^2 * e
		FVector x = pos2 - pos1;
		FVector forceDir = x.GetSafeNormal();
		FVector force = GravityConst * (Mass * other->Mass) / x.SizeSquared() * forceDir;

		DrawDebugDirectionalArrow(GetWorld(), pos1, pos1 + force.GetSafeNormal() * DebugArrowLength, 100, FColor::Red);
	}

	DrawDebugVelocityVector();
}


void ACelestialBody::UpdatePosition(float DeltaTime)
{
	FQuat rotation = FQuat(1.0, 1.0, 1.0, 1.0);
	RootComponent->MoveComponent(Velocity * DeltaTime, rotation, false);
}

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

	//WIP PROTOTYPE - INITIAL POSITIONING FOR ORBITAL BODIES
	if (CalculateInitialVelocityAndPosition)
	{
		int r = 3840; //promien orbity

		float v = InitialVelocityMagnitude;//sqrt(G * M) / r; - orbital velocity magnitude

		//initial position (sample position on circle)
		double posX = r * cos(0);
		double posY = r * sin(0);

		double velX = -v * sin(0); //v * cos(fi) + pi/2
		double velY = v * cos(0);  //v * sin(fi) + pi/2

		RootComponent->SetWorldLocation(FVector(posX, posY, 0));
		InitialVelocity = FVector(velX, velY, 0);
	}

	Velocity = InitialVelocity;
}

// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

