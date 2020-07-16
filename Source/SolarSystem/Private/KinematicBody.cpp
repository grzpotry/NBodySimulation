#include "SolarSystem/Public/KinematicBody.h"

FKinematicBody::FKinematicBody(): Mass(0)
{
}

FKinematicBody::FKinematicBody(const FVector& position_, const FVector& velocity_, const float mass_): Mass(mass_),Velocity(velocity_),Position(position_)
{
}

FVector FKinematicBody::CalculateVelocity(TArray<FKinematicBody> otherBodies, float GravityConst, float DeltaTime) const
{
	FVector newVelocity = Velocity;

	for (FKinematicBody other : otherBodies)
	{
		//F = G(m1m2)/r^2 * e
		FVector x = other.Position - Position;

		if (x == FVector::ZeroVector)
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