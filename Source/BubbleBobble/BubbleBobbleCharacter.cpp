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

	// Create a camera boom attached to the root (capsule)
	/*CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);*/
	

	// Create an orthographic camera (no perspective) and attach it to the boom
	/*SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 2048.0f;
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);*/

	// Prevent all automatic rotation behavior on the camera, character, and camera component
	/*CameraBoom->bAbsoluteRotation = true;
	SideViewCameraComponent->bUsePawnControlRotation = false;
	SideViewCameraComponent->bAutoActivate = true;*/
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

    // 	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IncarGear"));
    // 	TextComponent->SetRelativeScale3D(FVector(3.0f, 3.0f, 3.0f));
    // 	TextComponent->SetRelativeLocation(FVector(35.0f, 5.0f, 20.0f));
    // 	TextComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
    // 	TextComponent->SetupAttachment(RootComponent);

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

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if( GetSprite()->GetFlipbook() != DesiredAnimation 	)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}	

	
	/*switch (AnimationState)
	{
	case EAnimationStates::eIdle:	
		UE_LOG(LogTemp, Warning, TEXT("IDLE"));
		AnimationState = (PlayerSpeedSqr > 0.0f) ? EAnimationStates::eRunning : EAnimationStates::eIdle;
		GetSprite()->SetFlipbook(IdleAnimation);
		break;
	case EAnimationStates::eRunning:
		UE_LOG(LogTemp, Warning, TEXT("RUNNING"));
		GetSprite()->SetFlipbook(RunningAnimation);
		break;
	case EAnimationStates::eFiring:
		UE_LOG(LogTemp, Warning, TEXT("FIRING"));
		GetSprite()->SetFlipbook(FiringAnimation);
		break;
	case EAnimationStates::eJumping:
		UE_LOG(LogTemp, Warning, TEXT("JUMPING"));
		GetSprite()->SetFlipbook(JumpingAnimation);
		break;
	default:
		break;
	}*/
}

void ABubbleBobbleCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateCharacter();	
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
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

void ABubbleBobbleCharacter::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (GEngine) /** Global engine pointer. Can be 0 so don't use without checking. */
	{
		//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::White, "COLLISION");
	}

	// Checks actor is not ourself.
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
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

	}	
}

void ABubbleBobbleCharacter::Fire() //Shooting
{

	GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::White, "SHOT");

	/*AnimationState = EAnimationStates::eFiring;
	UE_LOG(LogTemp, Warning, TEXT("IT'S FIRING"));
	UpdateAnimation();
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		FRotator rotator;
		FVector spawnLocation = this->RootComponent->GetComponentLocation();

		ABubble* Bubble = World->SpawnActor<ABubble>(BubbleClass, spawnLocation, rotator, spawnParams);
		if (Bubble)
		{
			Bubble->FireInDirection(GetActorForwardVector());
		}
	}*/

}

void ABubbleBobbleCharacter::StopAnimation()
{
	AnimationState = EAnimationStates::eIdle;
	UpdateAnimation();
}

void ABubbleBobbleCharacter::Respawn()
{
	lives--;
	this->SetActorLocation(spawnPos, false);

	// INVINCIBLE FOR X TIME
}