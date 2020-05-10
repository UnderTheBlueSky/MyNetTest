// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BattlePlayerState.generated.h"

class ABattleCharacter;

/**
 * 
 */
UCLASS()
class MYNETTEST_API ABattlePlayerState : public APlayerState
{
	GENERATED_BODY()
	


public:

	ABattlePlayerState();

	ABattleCharacter* BattleCharacter;

	bool bIsBinded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
	float PlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	float MaxPlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	FLinearColor PlayerColor;

	UFUNCTION(Server, Reliable)
	void BindPawnHealthChange();

	UFUNCTION()
	void HealthChanged();

	


	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:

	virtual void BeginPlay() override;

};
