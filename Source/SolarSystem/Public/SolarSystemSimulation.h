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
class SOLARSYSTEM_API ASolarSystemSimulation : public ALevelScriptActor
{
	GENERATED_BODY()

	public:

	UPROPERTY(EditAnywhere, Category = Physics)
    float Gravity;

	UPROPERTY(VisibleAnywhere)
    TArray<ACelestialBody*> Bodies;

    // Called every frame
    virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	protected:

    UFUNCTION(BlueprintCallable, Category = LevelBlueprint)
    void DoSomething()
    {
    	GLog->Log("Does something");
    }
};
