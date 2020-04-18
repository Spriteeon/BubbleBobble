// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "BubbleBobbleCharacter.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/Engine.h"

#include "Bubble.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

//////////////////////////////////////////////////////////////////////////
// ABubbleBobbleCharacter

ABubbleBobbleCharacter::ABubbleBobbleCharacter()
{
	// Use only Yaw from the controller and ignore the rest of the rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule.
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;	// CHANGED (2.0f)
	GetCharacterMovement()->AirControl = 0.4f;		// CHANGED (0.8f)
	GetCharacterMovement()->JumpZVelocity = 1300.0f; // CHANGED (1000.0f)
	GetCharacterMovement()->GroundFriction = 8.0f;	// CHANGED (3.0f)
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 300.0f;	// CHANGED (600.0f)

	// Lock character motion onto the XZ plane, so the character can't move in or out of the screen
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
	// Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
	// behavior on the edge of a ledge versus inclines by setting this to true or false
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	// Enable replication on the Sprite component so animations show up when networked
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
}

//////////////////////////////////////////////////////////////////////////
// Animation

void ABubbleBobbleCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	if (AnimationState != DesiredAnimation)
	{
		AnimationState = DesiredAnimation;
		switch (AnimationState)
		{
		case EAnimationStates::eIdle:
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Idle");
			GetSprite()->SetFlipbook(IdleAnimation);
			break;
		case EAnimationStates::eRunning:
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "Run");
			GetSprite()->SetFlipbook(RunningAnimation);
			break;
		case EAnimationStates::eFiring:
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, "Fire!!");
			GetSprite()->SetFlipbook(FiringAnimation);
			break;
		case EAnimationStates::eJumping:
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Jump!!");
			GetSprite()->SetFlipbook(JumpingAnimation);
			break;
		default:
			break;
		}
	}
	else
	{
		if (AnimationState == EAnimationStates::eIdle || AnimationState == EAnimationStates::eRunning)
		{
			DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? EAnimationStates::eRunning : EAnimationStates::eIdle;
		}
	}
}

void ABubbleBobbleCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
	//get the game mode and do the timer
}

void ABubbleBobbleCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABubbleBobbleCharacter::OnOverlapBegin);

	spawnPos = this->GetActorLocation();
}


//////////////////////////////////////////////////////////////////////////
// Input

void ABubbleBobbleCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Note: the 'Jump' action and the 'MoveRight' axis are bound to actual keys/buttons/sticks in DefaultInput.ini (editable from Project Settings..Input)
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABubbleBobbleCharacter::MoveRight);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABubbleBobbleCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABubbleBobbleCharacter::StopAnimation);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ABubbleBobbleCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ABubbleBobbleCharacter::TouchStopped);
}

void ABubbleBobbleCharacter::MoveRight(float Value)
{
	/*UpdateChar();*/

	// Apply the input to the character motion
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}

void ABubbleBobbleCharacter::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Jump on any touch
	Jump();
}

void ABubbleBobbleCharacter::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// Cease jumping once touch stopped
	StopJumping();
}

void ABubbleBobbleCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();	
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f) //Left
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
			//The Other Darkness needs to shrink
		}
		else if (TravelDirection > 0.0f) //Right
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
			//The First Darkness needs to shrink
		}
	}
}

void ABubbleBobbleCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
//	if (GEngine) /** Global engine pointer. Can be 0 so don't use without checking. */
//	{
//		GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::White, "COLLISION");
//	}

	// Checks actor is not ourself.
	if ((OtherActor != nullptr) && (OtherActor != this) && !isImmune)
	{
		if (OtherActor->ActorHasTag("Enemy")) // Checks player is colliding with enemy
		{
			if (GEngine) /** Global engine pointer. Can be 0 so don't use without checking. */
			{
				GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::White, "ENEMY COLLISION");
			}

			if (lives <= 0)
			{
				// GAME OVER
				GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::White, "DEAD");
			}
			else
			{
				Respawn();
			}
		}
		if (OtherActor->ActorHasTag("Wall"))
		{



		}

	}	
}

void ABubbleBobbleCharacter::Fire() //Shooting
{
	DesiredAnimation = EAnimationStates::eFiring;
	UGameplayStatics::PlaySound2D(this, FireSound);
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		FVector spawnLocation = this->RootComponent->GetComponentLocation();

		ABubble* Bubble = World->SpawnActor<ABubble>(BubbleClass, spawnLocation, FRotator::ZeroRotator);
		if (currentPower == EPlayerPower::eStandard)
		{
			Bubble->currentBubble = EBubbleType::eStandardBubble;
		}
		if (currentPower == EPlayerPower::eBubblegum)
		{
			Bubble->currentBubble = EBubbleType::eGumBubble;
		}
		if (Bubble)
		{
			Bubble->FireInDirection(GetActorForwardVector());
		}
	}
}

void ABubbleBobbleCharacter::StopAnimation()
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->GetTimerManager().SetTimer(loopTimeHandle, this, &ABubbleBobbleCharacter::onTimerEnd, 0.6f, false);
	}
}

void ABubbleBobbleCharacter::StopAnimation(float argWaitingTime)
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->GetTimerManager().SetTimer(loopTimeHandle, this, &ABubbleBobbleCharacter::onTimerEnd, argWaitingTime, false);
	}
}

void ABubbleBobbleCharacter::Jump()
{
	UGameplayStatics::PlaySound2D(this, JumpSound);
	bPressedJump = true;
	JumpKeyHoldTime = 0.0f;
	DesiredAnimation = EAnimationStates::eJumping;
}

void ABubbleBobbleCharacter::StopJumping()
{
	bPressedJump = false;
	ResetJumpState();
	StopAnimation(1.f);
}

void ABubbleBobbleCharacter::Respawn()
{
	UGameplayStatics::PlaySound2D(this, RespawnSound);
	lives--;
	this->SetActorLocation(spawnPos, false);
	SetImmunity();
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		World->GetTimerManager().SetTimer(loopTimeHandle, this, &ABubbleBobbleCharacter::SetImmunity, 2.f, false);
	}
	// INVINCIBLE FOR X TIME
}

void ABubbleBobbleCharacter::onTimerEnd()
{
	DesiredAnimation = EAnimationStates::eIdle;
}