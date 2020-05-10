// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BattleCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NET/UnrealNetwork.h"
#include "GameSettings/BattlePlayerController.h"
#include "Components/Widget.h"
#include "Blueprint/UserWidget.h"
#include "CharacterAttributeIWidget.h"
#include "GameSettings/BattlePlayerController.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameSettings/BattleGameMode.h"
#include "GameFramework/PlayerState.h"


// Sets default values
ABattleCharacter::ABattleCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	Health = MaxHealth;

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);
}

void ABattleCharacter::Rep_HealthChange()
{
	if (Health < 0.0f) 
	{
		Health = 0.0f;
		IsDaed();//ËÀÍö
		return;
	}
	if (Health > 100.0f)
	{
		Health = 100.0f;
		HealthIsFull();//Ê¤Àû
		return;
	}
	HealthChanged.Broadcast();
}

void ABattleCharacter::UpdateOtherPlayerInfo()
{
	TArray<AActor*> TempPlayerStates;
	UGameplayStatics::GetAllActorsOfClass(GWorld, ABattlePlayerState::StaticClass(), TempPlayerStates);
	for (AActor* TempActor : TempPlayerStates)
	{
		ABattlePlayerState* TempPlayerState = Cast<ABattlePlayerState>(TempActor);
		TempPlayerState->BindPawnHealthChange();
		TempPlayerState->HealthChanged();
		PlayerStateList.Add(TempPlayerState);
	}
	UE_LOG(LogTemp, Warning, TEXT("UpdateOtherPlayerInfo"));
}

void ABattleCharacter::InitPlayersInfo()
{
	TArray<AActor*> Characters;
	UGameplayStatics::GetAllActorsOfClass(GWorld, ABattleCharacter::StaticClass(), Characters);

	for (AActor* TempActor : Characters)
	{
		ABattleCharacter* TempCharacter = Cast<ABattleCharacter>(TempActor);
		TempCharacter->UpdateOtherPlayerInfo();
	}
}

void ABattleCharacter::OpenSprint()
{
	
	if (GetWorldTimerManager().IsTimerPaused(HealthTimerHandle)) 
	{
		return;
	}
	SetMaxSpeed(1100.f);
	OpenTimerOnServer(MySprintTimerHandle);
	if (GetLocalRole() == ROLE_Authority)
	{
		SetParticleVisiblity_ToAll(true);
	}
	else
	{
		SetParticleVisiblity_OnServer(true);
	}
}

void ABattleCharacter::EndSprint()
{
	SetMaxSpeed(600.f);
	StopTimerOnServer(MySprintTimerHandle);
	if (GetLocalRole() == ROLE_Authority)
	{
		SetParticleVisiblity_ToAll(false);
	}
	else
	{
		SetParticleVisiblity_OnServer(false);
	}
}

void ABattleCharacter::LoseHealth_Implementation()
{
	Health -= 0.5;
	Rep_HealthChange();
	if (Health <= 0) 
	{
		if (GetLocalRole() == ROLE_Authority) 
		{
			PlayMontageToAll(DeathMontage);
		}
		else 
		{
			PlayMontageOnServer(DeathMontage);
		}
	}
}

void ABattleCharacter::ReducesHealthPerSecond()
{
	GetWorldTimerManager().UnPauseTimer(HealthTimerHandle);
}

void ABattleCharacter::IsDaed_Implementation()
{
	ABattleGameMode* GM = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GWorld));
	ABattlePlayerState* TempPlayerState = Cast<ABattlePlayerState>(GetPlayerState());
	GM->PlayerPawnDeath(this);
}

void ABattleCharacter::HealthIsFull_Implementation()
{
	ABattleGameMode* GM = Cast<ABattleGameMode>(UGameplayStatics::GetGameMode(GWorld));
	ABattlePlayerState* TempPlayerState = Cast<ABattlePlayerState>(GetPlayerState());
	GM->PlayerPawnHealthIsFull(this);
}

void ABattleCharacter::PlayMontageOnServer_Implementation(class UAnimMontage* InMontage, float InPlayRate = 1.f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength, float InTimeToStartMontageAt = 0.f, bool bStopAllMontages = true)
{
	PlayMontageToAll(InMontage, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
}

void ABattleCharacter::PlayMontageToAll_Implementation(class UAnimMontage* InMontage, float InPlayRate = 1.f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength, float InTimeToStartMontageAt = 0.f, bool bStopAllMontages = true)
{
	GetMesh()->GetAnimInstance()->Montage_Play(InMontage, InPlayRate, ReturnValueType, InTimeToStartMontageAt, bStopAllMontages);
}

void ABattleCharacter::StopTimerOnServer(FTimerHandle TimerHandle)
{
	GetWorldTimerManager().PauseTimer(TimerHandle);
}

void ABattleCharacter::OpenTimerOnServer(FTimerHandle TimerHandle)
{
	GetWorldTimerManager().UnPauseTimer(TimerHandle);
}

void ABattleCharacter::SetParticleVisiblity_ToAll_Implementation(bool IsVisible)
{
	ParticleSystem->SetVisibility(IsVisible);
}

void ABattleCharacter::SetParticleVisiblity_OnServer_Implementation(bool IsVisible)
{
	SetParticleVisiblity_ToAll(IsVisible);
}

void ABattleCharacter::SetMaxSpeed_Implementation(float NewMaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxSpeed;
}

void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!GetController()) 
	{
		return;
	}
	if (IsLocallyControlled())
	{
		ABattlePlayerController* TempPC = Cast<ABattlePlayerController>(Controller);
		TempPC->InitAttributes();
	}
	GetWorldTimerManager().SetTimer(MySprintTimerHandle, this, &ABattleCharacter::LoseHealth, 0.1f, true);
	GetWorldTimerManager().PauseTimer(MySprintTimerHandle);
	GetWorldTimerManager().SetTimer(HealthTimerHandle, this, &ABattleCharacter::LoseHealth, 0.1f, true);
	GetWorldTimerManager().PauseTimer(HealthTimerHandle);
}

void ABattleCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		FRotator ControlerRotaion = Controller->GetControlRotation();
		FRotator YawRotation(0, ControlerRotaion.Yaw, 0);
		
		const FVector MoveDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(MoveDirection, Value);
	}
}

void ABattleCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		FRotator ControlerRotaion = Controller->GetControlRotation();
		FRotator YawRotation(0, ControlerRotaion.Yaw, 0);

		const FVector MoveDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(MoveDirection, Value);
	}
}


// Called to bind functionality to input
void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABattleCharacter::OpenSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABattleCharacter::EndSprint);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABattleCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABattleCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void ABattleCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABattleCharacter, Health);
}

