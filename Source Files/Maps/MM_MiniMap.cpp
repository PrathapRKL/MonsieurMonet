// Fill out your copyright notice in the Description page of Project Settings.

#include "MM_MiniMap.h"
#include "Runtime/Engine/Classes/Components/SceneCaptureComponent2D.h"
#include "ProceduralMeshComponent.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Components/SphereComponent.h"

// Sets default values
AMM_MiniMap::AMM_MiniMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//RootComponent = CollisionComponent;

	//// Create the line of sight mesh
	//LOSMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("LineOfSightMesh"));
	//LOSMesh->bUseAsyncCooking = true;
	//LOSMesh->ContainsPhysicsTriMeshData(false);
	////LOSMesh->AttachToComponent(RootComponent, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	//LOSMesh->AttachTo(RootComponent);

	//minimapCam
	MinimapCam = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapCam"));
	MinimapCam->ProjectionType = ECameraProjectionMode::Orthographic;
	MinimapCam->OrthoWidth = 2000.0F;
	RootComponent = MinimapCam;
}

// Called when the game starts or when spawned
void AMM_MiniMap::BeginPlay()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	Super::BeginPlay();

	////attach this actor to the player character
	//ACharacter* CurrentPlayer = UGameplayStatics::GetPlayerCharacter(world, 0);
	//if (CurrentPlayer)
	//{
		//MinimapCam->TextureTarget = RenderT;
		//AttachToComponent(CurrentPlayer->RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	//	
	//}

	//this does not let the actor change its rotation
	//MinimapCam->SetAbsolute(false, true, true);
}

// Called every frame
void AMM_MiniMap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}