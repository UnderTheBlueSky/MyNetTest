// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/BattleGameMode.h"
#include "GameSettings/BattlePlayerController.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "NET/OnlineEngineInterface.h"
#include "GameFramework/GameSession.h"
#include "Character/DeathActor.h"
#include "Character/BattleCharacter.h"

ABattleGameMode::ABattleGameMode()
{
	MaxPlayerNum = 4;
}

void ABattleGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	// Login unique id must match server expected unique id type OR No unique id could mean game doesn't use them
	const bool bUniqueIdCheckOk = (!UniqueId.IsValid() || (UniqueId.GetType() == UOnlineEngineInterface::Get()->GetDefaultOnlineSubsystemName()));
	if (bUniqueIdCheckOk)
	{
		ErrorMessage = GameSession->ApproveLogin(Options);
	}
	else
	{
		ErrorMessage = TEXT("incompatible_unique_net_id");
	}
	if (NumPlayers == MaxPlayerNum)
	{
		ErrorMessage = TEXT("Player_count_is_full");
	}

	FGameModeEvents::GameModePreLoginEvent.Broadcast(this, UniqueId, ErrorMessage);
}

void ABattleGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (GetLocalRole() == ROLE_Authority) 
	{
		ABattlePlayerController* PC = Cast<ABattlePlayerController>(NewPlayer);
		PC->StartPlayerPawn();
		AGameStateBase* TempGameState = GetWorld()->GetGameState();
		int PlayerCount = TempGameState->PlayerArray.Num();
		switch (PlayerCount)
		{
		case 1:
			PC->PlayerName = TEXT("Player1");
			PC->PlayerState->SetPlayerName(TEXT("Player1"));
			break;
		case 2:
			PC->PlayerName = TEXT("Player2");
			PC->PlayerState->SetPlayerName(TEXT("Player2"));
			break;
		case 3:
			PC->PlayerName = TEXT("Player3");
			PC->PlayerState->SetPlayerName(TEXT("Player3"));
			break;
		case 4:
			PC->PlayerName = TEXT("Player4");
			PC->PlayerState->SetPlayerName(TEXT("Player4"));
			break;
		case 5:
			PC->PlayerName = TEXT("Player5");
			PC->PlayerState->SetPlayerName(TEXT("Player5"));
			break;
		case 6:
			PC->PlayerName = TEXT("Player6");
			PC->PlayerState->SetPlayerName(TEXT("Player6"));
			break;
		case 7:
			PC->PlayerName = TEXT("Player7");
			PC->PlayerState->SetPlayerName(TEXT("Player7"));
			break;
		case 8:
			PC->PlayerName = TEXT("Player8");
			PC->PlayerState->SetPlayerName(TEXT("Player8"));
			break;
		case 9:
			PC->PlayerName = TEXT("Player9");
			PC->PlayerState->SetPlayerName(TEXT("Player9"));
			break;
		case 10:
			PC->PlayerName = TEXT("Player10");
			PC->PlayerState->SetPlayerName(TEXT("Player10"));
			break;
		}
	}
}

void ABattleGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();

}

bool ABattleGameMode::ReadyToStartMatch_Implementation()
{
	// If bDelayed Start is set, wait for a manual match start
	if (bDelayedStart)
	{
		return false;
	}

	// By default start when we have > 0 players
	if (GetMatchState() == MatchState::WaitingToStart)
	{
		if (NumPlayers + NumBots == MaxPlayerNum)
		{
			return true;
		}
	}
	return false;
}
// 
// void ABattleGameMode::PlayerPawnDeath(ABattleCharacter* Character, FTransform PawnTransform, FLinearColor Color)
// {
// 	FActorSpawnParameters SpawnParameters;
// 	SpawnParameters.SpawnCollisionHandlingOverride =ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
// 	ADeathActor* DeathActor = GetWorld()->SpawnActor<ADeathActor>(ADeathActor::StaticClass(), PawnTransform, SpawnParameters);
// 	DeathActor->Color = Color;
// 	Character->Destroy();
// }

