// Fill out your copyright notice in the Description page of Project Settings.
//https://docs.unrealengine.com/en-US/Programming/Tutorials/FirstPersonShooter/3/1/index.html

#include "Bubble.h"
#include "BubbleBobble.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABubble::ABubble()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//// Use a sphere as a simple collision representation.
	//CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//// Set the sphere's collision radius.
	//CollisionComponent->InitSphereRadius(15.0f);
	//// Set the root component to be the collision component.
	//RootComponent = CollisionComponent;

	// Use this component to drive this projectile's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	/*ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);*/
	ProjectileMovementComponent->InitialSpeed = 250.0f;
	ProjectileMovementComponent->MaxSpeed = 250.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;

}

void ABubble::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// Called when the game starts or when spawned
void ABubble::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABubble::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

}
