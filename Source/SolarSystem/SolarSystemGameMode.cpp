// Copyright Epic Games, Inc. All Rights Reserved.

#include "SolarSystemGameMode.h"
#include "SolarSystemBall.h"

ASolarSystemGameMode::ASolarSystemGameMode()
{
	// set default pawn class to our ball
	DefaultPawnClass = ASolarSystemBall::StaticClass();
}
