// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameSettings/BattlePlayerState.h"
#include "BattleCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ABattlePlayerController;
class UParticleSystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthChangeDelegate);



UCLASS()
class MYNETTEST_API ABattleCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Particle")
	class UParticleSystemComponent* ParticleSystem;

public:
	// Sets default values for this character's properties
	ABattleCharacter();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<ABattlePlayerState*> PlayerStateList;

	UPROPERTY(BlueprintAssignable)
	FHealthChangeDelegate HealthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,ReplicatedUsing=Rep_HealthChange)
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle MySprintTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimerHandle HealthTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* DeathMontage;

	UFUNCTION(BlueprintCallable)
	void Rep_HealthChange();

	void UpdateOtherPlayerInfo();

	UFUNCTION(BlueprintCallable)
	void InitPlayersInfo();

	void OpenSprint();

	void EndSprint();

	UFUNCTION(Server, Reliable)
	void SetMaxSpeed(float NewMaxSpeed);

	UFUNCTION(Server, Reliable)
	void SetParticleVisiblity_OnServer(bool IsVisible);

	UFUNCTION(NetMulticast, Reliable)
	void SetParticleVisiblity_ToAll(bool IsVisible);

	UFUNCTION(Server, Reliable)
	void LoseHealth();

	UFUNCTION(BlueprintCallable)
	void OpenTimerOnServer(FTimerHandle TimerHandle);

	UFUNCTION(BlueprintCallable)
	void StopTimerOnServer(FTimerHandle TimerHandle);

	UFUNCTION(BlueprintCallable)
	void ReducesHealthPerSecond();

	UFUNCTION(Server, Reliable)
	void PlayMontageOnServer(class UAnimMontage* InMontage, float InPlayRate = 1.f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength, float InTimeToStartMontageAt = 0.f, bool bStopAllMontages = true);

	UFUNCTION(NetMulticast,Reliable)
	void PlayMontageToAll(class UAnimMontage* InMontage, float InPlayRate = 1.f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength, float InTimeToStartMontageAt = 0.f, bool bStopAllMontages = true);

	UFUNCTION(Server, Reliable)
	void IsDaed();

	UFUNCTION(Server, Reliable)
	void HealthIsFull();



protected:

	virtual void BeginPlay() override;

	void MoveForward(float Value);

	void MoveRight(float Value);

public:	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UParticleSystemComponent* GetParticleSystem() const { return ParticleSystem; }
};
