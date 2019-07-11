// This code belongs to Learning Machines.

#include "MM_Cheese.h"
#include "MonsieurMonet_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMM_Cheese::AMM_Cheese()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMM_Cheese::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMM_Cheese::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMM_Cheese::EatCheese()
{
	FTimerHandle TimerHandle;
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCharacter->PlayAnimMontage(PlayerCharacter->EatingMontage, 1.0f);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, PlayerCharacter->EatingMontage->GetPlayLength(), false, PlayerCharacter->EatingMontage->GetPlayLength());
	OnEatCheese();
}

void AMM_Cheese::OnEatCheese()
{
	AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerCharacter->Health += 0.25;
	PlayerCharacter->CheeseCount -= 1;
}

