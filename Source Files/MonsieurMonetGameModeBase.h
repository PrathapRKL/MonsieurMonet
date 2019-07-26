// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MonsieurMonetGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MONSIEURMONET_API AMonsieurMonetGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
		virtual void BeginPlay() override;
public:
	AMonsieurMonetGameModeBase();

	//Enum states for the different UIs in the game.
	enum EHUD_State : uint8
	{
		HS_InGame,
		HS_Inventory,
		HS_Map,
		HS_PauseGame
	};

	//Checks the HUD states and calls the ApplyHUD function to apply the specified HUD.
	void ApplyHUDChanges();

	//Getter function for the HUD states.
	uint8 GetHudState();

	//Setter function for the HUD states to call the corresponding function.
	UFUNCTION(BlueprintCallable)
		void ChangeHUDState(uint8 State);

	//Applies a HUD widget/UI to the screen. //change the mouse values to controller specific later.
	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents);

protected:
	uint8 HUDState;
	//The HUD widget/UI to be shown in-game.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> IngameHUDclass;

	//The HUD widget/UI to be shown when the inventory is opened.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> InventoryHUDclass;

	//The HUD widget/UI to be shown when the Map is opened.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> MapHUDclass;

	//The HUD widget/UI to be shown when the game is paused.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> PauseHUDclass;

	//Widget class.
	UPROPERTY()
		class UUserWidget* CurrentWidget;

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* LureItemDB;

	class AMM_Env_Interactables* Env_Interactables;

public:
	class UDataTable* GetLureItemDB() const { return LureItemDB; }

	//Notifying the guards that the player interacted with the environment interactables.
	UFUNCTION(BlueprintCallable)
		void OnEnvInteracted(AActor* OwnerActor);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void OnEnvInteractedEvent(AActor* OwnerActor);
};
