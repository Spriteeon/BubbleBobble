// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "Engine/Classes/Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Platform_BP2_Electric_Boogaloo.generated.h"

UCLASS()
class BUBBLEBOBBLE_API APlatform_BP2_Electric_Boogaloo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlatform_BP2_Electric_Boogaloo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = true))
	UPaperSpriteComponent* sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = true))
	UPaperSprite* sprite1;

	UBoxComponent* colBox;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = true))
	//UPaperSprite* sprite2;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = true))
	//UPaperSprite* sprite3;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = true))
	//UPaperSprite* sprite4;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = true))
	//UPaperSprite* sprite5;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = true))
	//UPaperSprite* sprite6;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = true))
	//UPaperSprite* sprite6;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta = (AllowPrivateAccess = true))
	//UPaperSprite* sprite6;



	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sprite, meta=(AllowPrivateAccess = true))
	//int isprite;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
