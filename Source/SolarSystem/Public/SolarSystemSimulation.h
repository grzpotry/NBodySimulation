// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CelestialBody.h"
#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "SolarSystemSimulation.generated.h"

/**
 *
 */
UCLASS()
class SOLARSYSTEM_API ASolarSystemSimulation : public AActor
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere, Category = Physics)
    float Gravity;

	UPROPERTY(EditAnywhere, Category = Debug)
	float PathForecastLength;

	UPROPERTY(EditAnywhere, Category = Debug)
	float MassMultiplier;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bSimulate = false;

	UPROPERTY(EditAnywhere, Category = Debug, meta=(Description="test desc"))
	float TrajectorySamplingMultiplier;

	UPROPERTY(VisibleAnywhere)
    TArray<ACelestialBody*> Bodies;

	void DrawTrajectories(TArray<FKinematicBody> kinematicBodies) const;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

public:
	ASolarSystemSimulation()
	{
		PrimaryActorTick.bCanEverTick = true;
	}
protected:
    UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
    void DoSomething()
    {
    	GLog->Log("Does something");
    }
};
