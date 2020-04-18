// Fill out your copyright notice in the Description page of Project Settings.

#include "BubbleBobbleGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UBubbleBobbleGameInstance::BG_Music()
{
	if (Music != NULL)
	{
		if (Music->PitchMultiplier > 1.0f)
		{
			Music->SetPitchMultiplier(1.0f);
		}		
	}	
	FString levelName = UGameplayStatics::GetCurrentLevelName(this);
	if (levelName == "Level_7")
	{
		Music->Stop();
		Music = UGameplayStatics::SpawnSound2D(this, FearModeMusic, 1.0f, 1.0f, 0.0f, nullptr, true, true);
	}
	else
	{
		Music = UGameplayStatics::SpawnSound2D(this, NormalModeMusic, 1.0f, 1.0f, 0.0f, nullptr, true, true);
	}	
}
