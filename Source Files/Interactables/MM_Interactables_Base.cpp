// Fill out your copyright notice in the Description page of Project Settings.

#include "MM_Interactables_Base.h"

// Sets default values
AMM_Interactables_Base::AMM_Interactables_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	InteractableHelpText = FString("Please Interact");
}

// Called when the game starts or when spawned
void AMM_Interactables_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMM_Interactables_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMM_Interactables_Base::PUInteract_Implementation()
{

}

//void AMM_Interactables_Base::PUDiscard()
//{
//}

