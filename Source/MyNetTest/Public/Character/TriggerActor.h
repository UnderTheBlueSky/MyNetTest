// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerActor.generated.h"

class USphereComponent;


UCLASS(config=Game)
class MYNETTEST_API ATriggerActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta = (AllowPrivateAccess = "true"))
	class USphereComponent* ColloisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleSystem;
	
	

public:	
	// Sets default values for this actor's properties
	ATriggerActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BasicDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HealthReward;


	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE class USphereComponent* GetColloisionComponent() const { return ColloisionComponent; }
	FORCEINLINE class UParticleSystemComponent* GetParticleSystem() const { return ParticleSystem; }
};
