// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsieurMonetGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "MonsieurMonet_Character.h"
#include "Interactables/MM_Env_Interactables.h"

void AMonsieurMonetGameModeBase::BeginPlay()
{
	//Applies HUD to the screen on BeginPlay (At the start of the game).
	ApplyHUDChanges();
	//Env_Interactables->OnInteracted.AddDynamic(this, &AMonsieurMonetGameModeBase::OnEnvInteracted);
}

AMonsieurMonetGameModeBase::AMonsieurMonetGameModeBase()
{
	//Setting up the default player pawn as MonsieurMonet_Character.
	/*static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Characters/MonsieurMonet/BP/BP_MonsieurMonet_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;*/

		//Sets the default HUD state to In-Game.
		HUDState = EHUD_State::HS_InGame;
	/*}*/
}

void AMonsieurMonetGameModeBase::ApplyHUDChanges()
{
	//Check and remove the previous or existing HUD and apply a new one.
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
	}

	switch (HUDState)
	{
	case EHUD_State::HS_InGame:
	{
		ApplyHUD(IngameHUDclass, false, false);
		break;
	}
	case EHUD_State::HS_Inventory:
	{
		ApplyHUD(InventoryHUDclass, true, true);
		break;
	}
	case EHUD_State::HS_Map:
	{
		ApplyHUD(MapHUDclass, false, false);
		break;
	}
	case EHUD_State::HS_PauseGame:
	{
		ApplyHUD(PauseHUDclass, true, true);
		break;
	}
	default:
	{
		ApplyHUD(IngameHUDclass, false, false);
		break;
	}
	}
}

uint8 AMonsieurMonetGameModeBase::GetHudState()
{
	return HUDState;
}

void AMonsieurMonetGameModeBase::ChangeHUDState(uint8 State)
{
	HUDState = State;
	ApplyHUDChanges();
}

bool AMonsieurMonetGameModeBase::ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents)
{
	//Get a reference to the Player and the PlayerController.
	class AMonsieurMonet_Character* PlayerCharacter = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(this, 0));
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	//NullChecking the widgets we want to apply.
	if (WidgetToApply != nullptr)
	{
		//Set mouse visibility and clickevents  accoeding to the parameters.
		PlayerController->bShowMouseCursor = bShowMouseCursor;
		PlayerController->bEnableClickEvents = EnableClickEvents;

		//Creating the widget.
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);

		//Checking to make sure if the widget has been applied properly.
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void AMonsieurMonetGameModeBase::OnEnvInteracted(AActor * OwnerActor)
{
	/*OnEnvInteractedEvent(this);*/
}
