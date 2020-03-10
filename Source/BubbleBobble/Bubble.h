// Fill out your copyright notice in the Description page of Project Settings.

//Use this for floating
//https://subscription.packtpub.com/book/game_development/9781785286018/1/ch01lvl1sec12/adding-movement


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bubble.generated.h"

UCLASS()
class BUBBLEBOBBLE_API ABubble : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABubble();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle floatTimer;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Floating)
	bool floating{ false };

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Sphere Collision Component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Projectile movement component.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement)
	class UFloatingPawnMovement* FloatingPawnMovement;

	void FireInDirection(const FVector& ShootDirection);
	void Despawn();
	void Float();

};
