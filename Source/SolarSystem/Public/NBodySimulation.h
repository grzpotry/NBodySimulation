// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CelestialBody.h"
#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "NBodySimulation.generated.h"

/**
 *
 */
UCLASS()
class SOLARSYSTEM_API ANBodySimulation : public AActor
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere, Category = Physics)
    float Gravity;

	UPROPERTY(EditAnywhere, Category = Debug)
	float OrbitForecastLength;

	UPROPERTY(EditAnywhere, Category = Debug)
	float MassMultiplier;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bSimulate = false;

	UPROPERTY(EditAnywhere, Category = Debug)
	bool bDrawOrbits = true;

	UPROPERTY(EditAnywhere, Category = Debug)
	float SpeedFactor = 1.0;

	UPROPERTY(EditAnywhere, Category = Debug, meta=(Description="test desc"))
	float OrbitSamplingMultiplier;

	UPROPERTY(EditInstanceOnly, Category = Physics)
	ACelestialBody * CentralBody;

	UPROPERTY(EditAnywhere)
    TArray<ACelestialBody*> Bodies;

	void UpdatePredictedOrbits(TArray<FKinematicBody>& kinematicBodies);

    // Called every frame
    virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

public:
	ANBodySimulation()
	{
		PrimaryActorTick.bCanEverTick = true;
	}
protected:
    UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
    void DoSomething()
    {
    	GLog->Log("Does something");;
    }

private:
	void UpdatePredictedOrbitsStep(TArray<FKinematicBody>& kinematicBodies, FVector centralBodyInitPosition, int stepIndex);
};
