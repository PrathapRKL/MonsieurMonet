// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MM_MiniMap.generated.h"

class UMaterial;
class UProceduralMeshComponent;
class USceneCaptureComponent2D;
class UTextureRenderTarget2D;
class USphereComponent;

UCLASS()
class MONSIEURMONET_API AMM_MiniMap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMM_MiniMap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	USphereComponent* CollisionComponent;

public:
	UPROPERTY()
		USceneCaptureComponent2D* MinimapCam;

	UPROPERTY()
		UTextureRenderTarget2D* RenderT;
};