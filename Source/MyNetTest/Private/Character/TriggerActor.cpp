// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TriggerActor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Character/BattleCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"

// Sets default values
ATriggerActor::ATriggerActor()
{
    ColloisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ColloisionComponent"));
    ColloisionComponent->InitSphereRadius(60.f);
    ColloisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATriggerActor::OnOverlap);
    RootComponent = ColloisionComponent;

    ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
    ParticleSystem->SetupAttachment(ColloisionComponent);

	BasicDamage = 20.f;

	HealthReward = 20.f;

	bReplicates = true;
}

void ATriggerActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (GetLocalRole() == ROLE_Authority)
		{
			//让其他玩家受伤，自己加血
			ABattleCharacter* CurrentCharacter = Cast<ABattleCharacter>(OtherActor);
			if (CurrentCharacter) 
			{
				CurrentCharacter->Health += HealthReward;//生命值奖励
				Destroy();
			}
		}
    }
}

