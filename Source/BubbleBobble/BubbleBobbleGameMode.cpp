// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BubbleBobbleGameMode.h"
#include "BubbleBobbleCharacter.h"

ABubbleBobbleGameMode::ABubbleBobbleGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = ABubbleBobbleCharacter::StaticClass();	
}
