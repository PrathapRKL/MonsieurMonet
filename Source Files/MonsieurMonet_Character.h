// This code belongs to Learning Machines.

#pragma once

#include "Engine.h"
#include "GameFramework/Character.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Interactables/MM_CoverActor.h"
#include "Engine/Classes/GameFramework/DamageType.h"
//#include "UObject/ObjectMacros.h"
//#include "UObject/Object.h"
#include "Components/TimelineComponent.h"
//#include "Components/DecalComponent.h"
//#include "Engine/DecalActor.h"
#include "MonsieurMonet_Character.generated.h"

//Forward declarations.
class USpringArmComponent;
class UCameraComponent;
class AMM_Pickups;
class UTexture2D;
class USkeletalMeshComponent;
class AMM_FreezeProjectile;
class UCameraShake;
class UForceFeedbackEffect;
class UDataTable;
class AMM_LureItems_Base;
class USphereComponent;
class UBoxComponent;
class UDecalComponent;
class AMM_Cheese;
class UAkAudioEvent;
class UAkComponent;
class UInstancedStaticMeshComponent;

//Struct for Inventory.
USTRUCT(BlueprintType)
struct FLureItemInventory : public FTableRowBase
{
	GENERATED_BODY()

public:

	FLureItemInventory()
	{
		Name = FText::FromString("ItemName");
		Action = FText::FromString("Use");
		Description = FText::FromString("Please enter a description for the item.");
	}

	//ID of the lure item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int ItemID;

	//Array of LureItem class
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AMM_LureItems_Base> LureItemPickup;
	/*TMap<int32, LureItemPickup> LureItemsMap;*/

	//Name of the item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Name;

	//Amount of noise produced by the item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NoiseAmount;

	//Action the item can perform.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Action;

	//thumbnail of the item.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;

	//Description of the item - optional.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText Description;

	//Check to see if the item can be used.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanBeUsed;

	//operator overloading to compare data to notify unreal c++ of the item being used or deleted.
	bool operator==(const FLureItemInventory& Item) const
	{
		if (ItemID == Item.ItemID)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrowCoinSignature, AActor*, Actor);

UCLASS()
class MONSIEURMONET_API AMonsieurMonet_Character : public ACharacter
{
	GENERATED_BODY()

		/*ENGINE_API virtual void AddDia(FName DiaName, const FTableDiaBase& DiaData);*/

private:

	//The movement direction while taking cover;
	FVector CoverMovementDirection;

	float CoverValue;

public:

	// Sets default values for this character's properties
	AMonsieurMonet_Character();

	UPROPERTY(BlueprintAssignable)
		FOnThrowCoinSignature OnThrowCoin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 LureItemCount;

	//True if player can take cover.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bCanTakeCover = false;

	//True if player is currently taking cover.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsInCover = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bWantToCover = false;


	//float variables for controlling the cameras on hiding spots or cover actors.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CamPitch = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CamYaw = 0.0f;


	//CoverActor reference.
	AMM_CoverActor* Covers;

	//Health of the player character.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Health = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NoiseLevel;

	//Initializing the SpringArm Component for the camera.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		USpringArmComponent* CameraBoom;

	//Initializing the Camera Component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		UCameraComponent* PlayerCamera;

	struct FPostProcessSettings PostProcessSettings;

	//Creating a Static Mesh for the Freeze gun.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FreezeGun")
		UStaticMeshComponent* FreezeGun;

	//CollisionComponent to determine noise levels for the player. - to notify the guard.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* NoiseRadiusComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NoiseRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FreezeGun")
		TSubclassOf<AMM_FreezeProjectile> ProjectileToUse;

	//Camera shake while firing (for the freeze projectile).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UCameraShake> FireCameraShake;

	//ForceFeedback when firing.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UForceFeedbackEffect* FireForceFeedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FreezeCooldownTime = 12.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FreezeDuration = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentTime = 12.0f;

	//Yaw values.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TurnRate;

	//Pitch values.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float LookUpRate;

	FTimerHandle CooldownTimerHandle;

	//The characters' inventory, represented as a TArray of the pickup objects.
	UPROPERTY(EditAnywhere)
		TArray<AMM_LureItems_Base*> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDetected;

	//Text for the interactable items.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artifact Values")
		FString PickupText;

	//Value of the items.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Artifact Values")
		int32 Gold;

	//Check to see if the iventory slot is empty.
	UPROPERTY(EditAnywhere)
		bool SlotEmpty;

	//Conditions for the different player mechanics. 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		bool bCrouching = false;

	// check to see if the player character can cover.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		bool bEnterCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		bool bWeaponized;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		bool bCrouchCover;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AnimInstance")
		bool bCrouch_Weapon;

	UPROPERTY(EditAnywhere)
		FVector RayLocation = FVector(60.0f, 0.0f, 0.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsShooting = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsAiming_Crouch;

	float AimCameraPos_F = 0;
	float AimCameraPos_R = 0;
	float Aim_CamLoc_A = 90;
	float Aim_CamLoc_B = 60;
	float Aim_LerpedLoc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USphereComponent* SearchRange;

	//Anim-Montages for shooting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting")
		UAnimMontage* ShootAnimationMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* Gun_Sphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector GunOffset = FVector(100.0f, 0.0f, -10.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCapsuleComponent* TakedownCapsule;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Timeline components for the Aim_Camera Transition.
	UPROPERTY()
		UTimelineComponent* Aim_CameraTimeline;

	UPROPERTY()
		UCurveFloat* Aim_FloatCurve;

	UFUNCTION()
		void Aim_TimelineCallback(float Value);

	UFUNCTION()
		void Aim_TimelineFinishedCallback();

	void Aim_PlayTimeline();
	void Aim_ReverseTimeline();

	//Nearest lure item for picking up lures
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AMM_LureItems_Base* NearestLureItem;

	//Current lure item that the player is holding
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FName CurrentLureItem;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Functions.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Speed;

	//Functions to Move the character to the desired direction.
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);

	//Controller Yaw and Pitch.
	UFUNCTION()
		void ControllerPitch(float Pitch);

	UFUNCTION()
		void ControllerYaw(float Yaw);

	//Function to make the character sprint. Called when held down.
	UFUNCTION()
		void Sprint();

	//Function to stop the character from sprinting. Called when released.
	UFUNCTION()
		void Sprint_Stop();

	UFUNCTION()
		void Interact();

	//Function to Interact with all Interactable Items.
	UFUNCTION()
		void ToggleInventory();

	//for lure throwing
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* LureStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UDecalComponent* TargetEndDecal;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FName SocketName = "Right_Fingers1_Jnt";

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UInstancedStaticMeshComponent* BallAim;

	//tickaim
	UFUNCTION()
		void TickAim();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool canAimCPP = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool showBallAimCPP = false;
			
	//Event to aim at the target.
	UFUNCTION(BlueprintNativeEvent)
		void Aim();
	virtual void Aim_Implementation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector AimTargetPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector ProjVel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FVector> HitArray;

	UPROPERTY(BlueprintReadWrite)
		int CurrentHitNum = 0;
	UPROPERTY(BlueprintReadWrite)
		int CurrentAimInst = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void AimStop();
	virtual void AimStop_Implementation();

	//To shoot the freeze gun's projectile.
	UFUNCTION(BlueprintNativeEvent)
		void Shoot();
	virtual void Shoot_Implementation();

	//the freeze gun's cooldown.
	UFUNCTION()
		void WeaponCooldown();

	UFUNCTION()
		void PauseGame();

	//Crouch and uncrouch.
	UFUNCTION(BlueprintCallable)
		void PlayerCrouch();

	UFUNCTION(BlueprintCallable)
		void PlayerCrouch_Stop();

	//Enter cover.
	UFUNCTION(BlueprintCallable)
		void Cover();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void CoverEvent();

	//Values to interpolate the camera.
	float CrouchRate = 0;
	float CrouchCamera_F = 0;
	float CrouchCamera_R = 0;

	bool Issprinting;

	//Values to interpolate the camera's field of view (FOV).
	float FieldOfView_F = 0;
	float FieldOfView_R = 0;


	//Add Items to Inventory.
	UFUNCTION()
		bool AddItemToInventory(AMM_LureItems_Base* Item);

	//Constantly check for Interactable items around the specified radius, around the player.
	UFUNCTION()
		void CheckForInteractables();

	//Update gold amount.
	UFUNCTION(BlueprintCallable, Category = "Interactable Artifacts")
		void UpdateGold(int32 Amount);

	//Gets the thumbnail for the given inventory slot.
	UFUNCTION(BlueprintCallable, Category = "Interactable Artifacts")
		UTexture2D* GetInventorySlotThumbnail(int32 Slot);

	//Gets the item's name for a given slot.
	UFUNCTION(BlueprintCallable, Category = "Interactable Artifacts")
		FString GivenNameAtInventorySlot(int32 Slot);

	//Discarding Items from a given slot.
	UFUNCTION(BlueprintCallable, Category = "Interactable Artifacts")
		void ThrowLures(int32 Slot);

	//Add item by item id.
	UFUNCTION(BlueprintCallable)
		void AddItemToInventoryByID(int ID);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<FLureItemInventory> LInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Coins = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Peels = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int HealthRegen = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Bombs = 0;

	UFUNCTION()
		void SelectCoin();
	UFUNCTION()
		void SelectPeel();
	UFUNCTION()
		void SelectHealthRegen();
	UFUNCTION()
		void SelectBomb();



	////Value of each of the pickable item.
	//UFUNCTION(BlueprintCallable, Category = "Interactable_Artifacts")
	//	int Value(int32 Slot);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class AMM_Interactables_Base* CurrentInteractable;
	class AMM_Pickups* PickupItemss;

	//Temporary search radius (for interactables).
	UFUNCTION(BlueprintCallable)
		void SearchRangeOnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*UFUNCTION(BlueprintCallable)
		void LimitCamera();*/

		//Inform the player if he can take cover.
	UFUNCTION(BlueprintCallable)
		void SetCanTakeCover(bool CanTakeCover, AMM_CoverActor* CoverActor);

	//Toggle Cover.
	UFUNCTION(BlueprintCallable)
		void ToggleCover();

	UFUNCTION(BlueprintCallable)
		void Stop_Cover();

	//Cover animation functions.
	UFUNCTION(BlueprintCallable)
		float CoverValueInput() { return CoverValue; }

	UFUNCTION(BlueprintCallable)
		bool IsInCover() { return bIsInCover; }

	//Function to regenerate health.
	UFUNCTION(BlueprintCallable)
		void StartHealthregen();

	UFUNCTION(Blueprintcallable)
		void ModifyHealth(float Amount, UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser, float &Health_out, bool &AddingHealth_out);

	//Properties and to notify the guards when taking them down.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanTakeDown;

	UFUNCTION(BlueprintCallable)
		void TakeDown(AActor* TDPawn);

	UFUNCTION(BlueprintCallable)
		void TakeDownCB(AActor* CBoxes);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* TakedownMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* TakedownCircuitMontage;

	TSubclassOf<APawn> Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAttacking;
	FTimerHandle PauseAnimHandle;
	UFUNCTION()
		void PauseAnimMontages();

	//Subclass of Swift guard for takedown functionality.
	TSubclassOf<class ASwift_GuardCharacter> SwiftGuard;

	//For takedown.
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex);

	//Pickup animations.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* PickupMontage;

	FTimerHandle PickupTimerHandle;

	UFUNCTION(BlueprintCallable)
		void PUPickedUp();
	UFUNCTION(BlueprintCallable)
		void LIPickedUp();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPickedUpP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bPickedUpL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsInteracting;

	//To disable movement when interacting.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsHiding;

private:

	UPROPERTY(EditAnywhere, Category = "Line of Sight", meta = (AllowPrivateAccess = "true"))
		bool IsLOSOn = false;

	UPROPERTY(EditAnywhere, Category = "Line of Sight", meta = (AllowPrivateAccess = "true"))
		float LOSHeight = 0.0F;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line of Sight", meta = (AllowPrivateAccess = "true"))
		class UProceduralMeshComponent* LOSMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line of Sight", meta = (AllowPrivateAccess = "true"))
		UMaterial* LOSMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line of Sight", meta = (AllowPrivateAccess = "true", UIMin = "1.0", UIMax = "360.0"))
		float ArcAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line of Sight", meta = (AllowPrivateAccess = "true", UIMin = "1.0", UIMax = "5.0"))
		float AngleStep;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Line of Sight", meta = (AllowPrivateAccess = "true", UIMin = "200", UIMax = "1000"))
		float Radius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line of Sight", meta = (AllowPrivateAccess = "true"))
		TArray<FVector> LOSVertices;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Line of Sight", meta = (AllowPrivateAccess = "true"))
		TArray<int32> LOSTriangles;

	UFUNCTION()
		void InitLOSMesh();
	UFUNCTION()
		void TickLOSMesh();
	UFUNCTION()
		void UpdateLOSMeshData(const TArray<FVector>& Vertices, const TArray<int32>& Triangles);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxStamina = 100.0F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentStamina = 100.0F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StaminaConsumeRate = 0.5F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StaminaRecoverRate = 0.3F;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanSprint = true;

	UFUNCTION()
		void UpdateStamina();

	//Function to notify the enemy pawn the noise produced by the player.
	UFUNCTION(BlueprintCallable)
		void NotifyPawnNoise();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void NotifyPawnNoise_Event();

	//Array to store all registered guards from the BP.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> RegisteredGuards;

	//Timer Handle for the settimer event, for NotifyPawnNoiseEvent.
	FTimerHandle NotifyHandle;


	//Timeline Component for the crouch camera.
	//Timneline Component to Play the Crouch camera transition animation.
	UPROPERTY()
		FTimeline CrouchCameraAnimation;

	//Deltatime(stepsize) for the timer calling the timeline tick.
	static const float DELTATIME;

	//Function which gets called from the Timer to call animTimeline.
	UFUNCTION()
		void TickCrouchTimeline(float Value);

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> CrouchCamTimelineDir;

	//Curve float for the timeline component.
	UPROPERTY()
		UCurveFloat* CrouchCamCurve;

	UPROPERTY()
		FTimerHandle TimelineFinishedHandle;

	//Called when the timeline finishes playing.
	UFUNCTION()
		void PlayCrouchCameraAnim();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AMM_CoverSystem> CoverSystems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMM_CoverSystem* CoverActors;


	//Timeline Component for Dynamic_Sprint_Camera.
	//Timneline Component to Play the sprint camera transition animation.
	UPROPERTY()
		FTimeline SprintCameraAnimation;

	//Function which gets called from the Timer to call animTimeline.
	UFUNCTION(BlueprintCallable)
		void TickSprintTL(float Value);

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> SprintCamTimelineDir;

	//Curve float for the timeline component.
	UPROPERTY()
		UCurveFloat* SprintCamCurve;

	UPROPERTY()
		FTimerHandle TlineFinishHandle_Sprint;

	//Called when the timeline finishes playing.
	UFUNCTION()
		void PlaySprintCameraAnim();

	UPROPERTY()
		bool StaminaIsLow;


	//Delegate Function to check for and notify the guards.
	UFUNCTION(BlueprintCallable)
		void OnNoiseBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		void OnNoiseEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	//Timeline Component for Dynamic_Crouch and Crouch walk_Camera.
	//Timneline Component to Play the Crouch camera transition animation.
	UPROPERTY()
		FTimeline CrouchWalkTL;

	//Function which gets called from the Timer to call animTimeline.
	UFUNCTION()
		void TickCWTimeline(float Value);

	UPROPERTY()
		TEnumAsByte<ETimelineDirection::Type> CWCamTimelineDir;

	//Curve float for the timeline component.
	UPROPERTY()
		UCurveFloat* CWCamCurve;

	UPROPERTY()
		FTimerHandle TlineFinishHandle_CW;

	//Called when the timeline finishes playing.
	UFUNCTION()
		void PlayCWCameraAnim();


	//Animation montage for eating cheese.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* EatingMontage;

	//Stop the movement when eating cheese.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsEatingCheese = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CheeseCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AMM_Cheese*> Cheese;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AMM_Cheese* CheeseActor;

	UFUNCTION(BlueprintCallable)
		void EatCheese();

	//AK Component for chewing cheese.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UAkAudioEvent* ChewingCheeseEvent;

	UFUNCTION(BlueprintCallable)
		void DamageReceived(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);


	//Camera Shake for the death of the player character.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<class UCameraShake> DeathCamShake;
	//Montage for the cahracter's death animation.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UAnimMontage* PlayerDeathMontage;
	//AK Component for Death anim of the player.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UAkAudioEvent* Player_Lose;
};