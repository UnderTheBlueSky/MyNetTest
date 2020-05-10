// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BattlePlayerController.generated.h"

class APlayerStart;
class UUserWidget;

/**
 * 
 */
UCLASS()
class MYNETTEST_API ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:	
	ABattlePlayerController();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attributes")
	TSubclassOf<UUserWidget> AttributesWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* AttributesWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> PlayerListWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* PlayerListWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Replicated)
	FString PlayerName;

	void StartPlayerPawn();

	APlayerStart* FindStartLocation(FName StartTag);

	UFUNCTION(BlueprintCallable)
	void InitAttributes();




	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
