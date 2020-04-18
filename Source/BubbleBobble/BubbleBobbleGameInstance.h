// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Containers/UnrealString.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "BubbleBobbleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BUBBLEBOBBLE_API UBubbleBobbleGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int Score{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	TArray<FString> LettersCollected;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	UAudioComponent* Music;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	USoundBase* FearModeMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audio)
	USoundBase* NormalModeMusic;

	UFUNCTION()
	void BG_Music();
	
};
