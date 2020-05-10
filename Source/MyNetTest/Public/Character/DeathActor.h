// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeathActor.generated.h"

UCLASS()
class MYNETTEST_API ADeathActor : public AActor
{
	GENERATED_BODY()
	
	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

public:	
	// Sets default values for this actor's properties
	ADeathActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimationAsset* NewAnimToPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInstanceDynamic* MaterialInstanceDynamic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* SourceMaterial;


public:
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Mesh; }
};
