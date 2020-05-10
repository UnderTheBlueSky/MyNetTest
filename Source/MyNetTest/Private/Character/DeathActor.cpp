// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DeathActor.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ADeathActor::ADeathActor()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->AlwaysLoadOnClient = true;
	Mesh->AlwaysLoadOnServer = true;
	Mesh->bOwnerNoSee = false;
	

}

