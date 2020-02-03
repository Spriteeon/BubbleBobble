// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"
#include "Engine/World.h"

UCLASS()
class BUBBLEBOBBLE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

private:
	float movement_delay;
	bool move_dir;

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Movement();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
