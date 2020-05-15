// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "BubbleBobble.h"
#include "Components/CapsuleComponent.h"

#include "Engine/Engine.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bHidden = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
}

void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->ActorHasTag("Bubble")) // Checks bubble is colliding with enemy
	{
		if (GEngine) /** Global engine pointer. Can be 0 so don't use without checking. */
		{
			//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::White, "BUBBLED");
			//OtherActor->Destroy();
		}

	}

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

