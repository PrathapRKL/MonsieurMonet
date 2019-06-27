// Fill out your copyright notice in the Description page of Project Settings.

#include "Brute_GuardCharacter.h"

// Sets default values
ABrute_GuardCharacter::ABrute_GuardCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABrute_GuardCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrute_GuardCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABrute_GuardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

