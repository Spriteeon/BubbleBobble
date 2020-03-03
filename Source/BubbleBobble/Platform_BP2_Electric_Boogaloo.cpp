// Fill out your copyright notice in the Description page of Project Settings.

#include "Platform_BP2_Electric_Boogaloo.h"

// Sets default values
APlatform_BP2_Electric_Boogaloo::APlatform_BP2_Electric_Boogaloo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(sprite);

	/*switch (isprite)
	{
	case 0:
		sprite->SetSprite(sprite1);
		break;
	case 1:
		sprite->SetSprite(sprite1);

		break;
	case 2:
		sprite->SetSprite(sprite1);

		break;
	case 3:
		sprite->SetSprite(sprite1);

		break;
	case 4:
		sprite->SetSprite(sprite1);

		break;
	case 5:
		sprite->SetSprite(sprite1);

		break;
	case 6:
		sprite->SetSprite(sprite1);

		break;
	case 7:
		sprite->SetSprite(sprite1);

		break;
	case 8:
		sprite->SetSprite(sprite1);

		break;
	case 9:
		sprite->SetSprite(sprite1);

		break;
	}*/
}

// Called when the game starts or when spawned
void APlatform_BP2_Electric_Boogaloo::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlatform_BP2_Electric_Boogaloo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

