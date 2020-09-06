#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KinematicBody.generated.h"

USTRUCT()
struct FKinematicBody
{
    GENERATED_USTRUCT_BODY()

    FVector Velocity;
    FVector Position;
    float Mass;

    FKinematicBody();

    FKinematicBody(const FVector& position_, const FVector& velocity_, const float mass_);

    FVector CalculateVelocity(float massMultiplier, TArray<FKinematicBody> otherBodies, float GravityConst, float DeltaTime) const;
};

