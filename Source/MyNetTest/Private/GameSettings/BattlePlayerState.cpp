// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/BattlePlayerState.h"
#include "NET/UnrealNetwork.h"
#include "Character/BattleCharacter.h"
#include "Delegates/MulticastDelegateBase.h"
#include "GameFramework/GameStateBase.h"

ABattlePlayerState::ABattlePlayerState()
{
	bIsBinded = false;
}

void ABattlePlayerState::BeginPlay()
{
	if (GetLocalRole() == ROLE_Authority) 
	{
		AGameStateBase* TempGameState = GetWorld()->GetGameState();
		int PlayerCount = TempGameState->PlayerArray.Num();
		switch (PlayerCount)
		{
		case 1:
			PlayerColor = FLinearColor(FColor(220.0, 20.0, 60.0));
			break;
		case 2:
			PlayerColor = FLinearColor(FColor(255.0, 0.0, 255.0));
			break;
		case 3:
			PlayerColor = FLinearColor(FColor(75.0, 0.0, 130.0));
			break;
		case 4:
			PlayerColor = FLinearColor(FColor(0.0, 0.0, 255.0));
			break;
		case 5:
			PlayerColor = FLinearColor(FColor(0.0, 255.0, 255.0));
			break;
		case 6:
			PlayerColor = FLinearColor(FColor(85.0, 107.0, 47.0));
			break;
		case 7:
			PlayerColor = FLinearColor(FColor(255.0, 215.0, 0.0));
			break;
		case 8:
			PlayerColor = FLinearColor(FColor(255.0, 255.0, 255.0));
			break;
		case 9:
			PlayerColor = FLinearColor(FColor(0.0, 0.0, 0.0));
			break;
		case 10:
			PlayerColor = FLinearColor(FColor(1.0, 0.425, 0.0));
			break;
		}
	}
}



void ABattlePlayerState::BindPawnHealthChange_Implementation()
{
	if (!bIsBinded) 
	{
		BattleCharacter = Cast<ABattleCharacter>(GetPawn());
		if(BattleCharacter)
		{
			BattleCharacter->HealthChanged.AddDynamic(this, &ABattlePlayerState::HealthChanged);
			bIsBinded = true;
		}
	}
}

void ABattlePlayerState::HealthChanged()
{
	if (BattleCharacter) 
	{
		PlayerHealth = BattleCharacter->Health;
		MaxPlayerHealth = BattleCharacter->MaxHealth;
	}
}

void ABattlePlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABattlePlayerState, PlayerHealth);
	DOREPLIFETIME(ABattlePlayerState, MaxPlayerHealth);
	DOREPLIFETIME(ABattlePlayerState, PlayerColor);
}
