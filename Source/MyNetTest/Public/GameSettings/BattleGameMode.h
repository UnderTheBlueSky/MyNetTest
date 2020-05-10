// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BattleGameMode.generated.h"

class ABattleCharacter;

/**
 * 
 */
UCLASS()
class MYNETTEST_API ABattleGameMode : public AGameMode
{
	GENERATED_BODY()
	

public:	

	ABattleGameMode();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int MaxPlayerNum;

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void HandleMatchIsWaitingToStart() override;

	virtual bool ReadyToStartMatch_Implementation() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerPawnDeath(ABattleCharacter* Character);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerPawnHealthIsFull(ABattleCharacter* Character);
};
