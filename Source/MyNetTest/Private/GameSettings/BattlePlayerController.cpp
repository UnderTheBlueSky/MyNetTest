// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettings/BattlePlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Character/BattleCharacter.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "UObject/Object.h"
#include "CharacterAttributeIWidget.h"
#include "NET/UnrealNetwork.h"



ABattlePlayerController::ABattlePlayerController()
{
	AttributesWidgetClass = UCharacterAttributeIWidget::StaticClass();
}

void ABattlePlayerController::StartPlayerPawn()
{
	APawn* ControledPawn = GetPawn();
	if (ControledPawn)
	{
		ControledPawn->Destroy();
	}
	UWorld* TempWorld = GetWorld();
	AGameStateBase* TempGameState = TempWorld->GetGameState();
	int PlayerCount = TempGameState->PlayerArray.Num();
	APlayerStart* Starter = NewObject<APlayerStart>();
	ABattleCharacter* NewPawn;
	UClass* PawnClass = TempWorld->GetAuthGameMode()->DefaultPawnClass;
	switch (PlayerCount)
	{
	case 1:
		Starter = FindStartLocation(FName(TEXT("Player1")));
		break;
	case 2:
		Starter = FindStartLocation(FName(TEXT("Player2")));
		break;
	case 3:
		Starter = FindStartLocation(FName(TEXT("Player3")));
		break;
	case 4:
		Starter = FindStartLocation(FName(TEXT("Player4")));
		break;
	case 5:
		Starter = FindStartLocation(FName(TEXT("Player5")));
		break;
	case 6:
		Starter = FindStartLocation(FName(TEXT("Player6")));
		break;
	case 7:
		Starter = FindStartLocation(FName(TEXT("Player7")));
		break;
	case 8:
		Starter = FindStartLocation(FName(TEXT("Player8")));
		break;
	case 9:
		Starter = FindStartLocation(FName(TEXT("Player9")));
		break;
	case 10:
		Starter = FindStartLocation(FName(TEXT("Player10")));
		break;
	}
	NewPawn = TempWorld->SpawnActor<ABattleCharacter>(PawnClass, FTransform(Starter->GetActorLocation()));
	Possess(NewPawn);
}

APlayerStart* ABattlePlayerController::FindStartLocation(FName StartTag)
{
	UWorld* WorldTemp = GetWorld();
	for (TActorIterator<APlayerStart> It(WorldTemp); It; ++It)
	{
		APlayerStart* PlayerPawnStart = *It;
		if (PlayerPawnStart->PlayerStartTag == FName(StartTag))
		{
			return PlayerPawnStart;
		}
	}
	//如果没有找到合适的出生点  生成一个(防止崩溃)
	APlayerStart* DefaultPawnStart = WorldTemp->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), FTransform(FVector(0.0f,0.0f,0.0f)));
	return DefaultPawnStart;
}

void ABattlePlayerController::InitAttributes()
{
	AttributesWidget = CreateWidget<UUserWidget>(this, AttributesWidgetClass);
	AttributesWidget->AddToViewport();
	PlayerListWidget = CreateWidget<UUserWidget>(this, PlayerListWidgetClass);
	PlayerListWidget->AddToViewport();
}

void ABattlePlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABattlePlayerController, PlayerName);
}
