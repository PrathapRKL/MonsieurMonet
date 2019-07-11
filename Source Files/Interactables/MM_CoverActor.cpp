// This code belongs to Learning Machines.

#include "MM_CoverActor.h"
#include "MonsieurMonet_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Engine/GameEngine.h"

// Sets default values
AMM_CoverActor::AMM_CoverActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialize Components.
	CoverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoverMesh"));
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));

	SetRootComponent(CoverMesh);
	BoxComp->SetupAttachment(CoverMesh);

	BoxComp->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AMM_CoverActor::BeginPlay()
{
	Super::BeginPlay();

	if (BoxComp)
	{
		//Register Overlap events.
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AMM_CoverActor::OnCompBeginOverlap);
		BoxComp->OnComponentEndOverlap.AddDynamic(this, &AMM_CoverActor::OnCompEndOverlap);
	}
}

void AMM_CoverActor::OnCompEndOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA<AMonsieurMonet_Character>())
	{
		//Inform the player that he isn't capable of taking cover.
		AMonsieurMonet_Character* Character = Cast<AMonsieurMonet_Character>(OtherActor);
		Character->SetCanTakeCover(false, nullptr);
	}
}

void AMM_CoverActor::OnCompBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA<AMonsieurMonet_Character>())
	{
		//Inform the player that he is capable of taking cover.
		AMonsieurMonet_Character* Character = Cast<AMonsieurMonet_Character>(OtherActor);
		Character->SetCanTakeCover(true, this);
	}
}

bool AMM_CoverActor::IsClosetoPlayer(FName SocketName)
{
	/*const FVector SocketDistance;*/
	//Perform RayTrace to check if the player is close to a socket.
	TArray<FHitResult> HitResults;
	const FVector StartLocation = CoverMesh->GetSocketLocation(SocketName);
	const FVector EndLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
	/*SocketDistance = FVector::Dist(StartLocation, EndLocation);*/
	FCollisionShape Shape;
	Shape.ShapeType = ECollisionShape::Line;
	GetWorld()->SweepMultiByChannel(HitResults, StartLocation, EndLocation, FQuat(), ECC_WorldDynamic, Shape);
	//Return if the raycast contains the character inside the hit result(s).
	return HitResults.ContainsByPredicate([&](FHitResult hitResult)
	{
		AActor* HitActor = hitResult.GetActor();
		return HitActor && HitActor->IsA<AMonsieurMonet_Character>();
	});
}


FName AMM_CoverActor::GetClosestSocket()
{
	const FName AvailableSockets[4] =
	{
		FName("ForwardSocket"),
		FName("BackwardSocket"),
		FName("RightSocket"),
		FName("LeftSocket")
	};

	//Find the socket closest to the player.
	for (uint8 SocketPtr = 0; SocketPtr < 4; SocketPtr++)
	{
		if (IsClosetoPlayer(AvailableSockets[SocketPtr]))
		{
			return AvailableSockets[SocketPtr];
		}
	}
	//failsafe.
	return AvailableSockets[3];
}

void AMM_CoverActor::DetermineMovementDirection(FVector & MovementDirection, FRotator & FacingDirection)
{
	FName ClosestSocket = GetClosestSocket();
	AMonsieurMonet_Character* Character = Cast<AMonsieurMonet_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	//Determine the movement and facing direction of the player.
	if (ClosestSocket.IsEqual("ForwardSocket"))
	{
		MovementDirection = -GetActorRightVector();
		FacingDirection = Character->GetActorRotation();/* + FRotator(0, 180, 0)*/
	}
	else if (ClosestSocket.IsEqual("BackwardSocket"))
	{
		MovementDirection = GetActorRightVector();
		FacingDirection = Character->GetActorRotation();
	}
	else if (ClosestSocket.IsEqual("RightSocket"))
	{
		MovementDirection = -GetActorForwardVector();
		FacingDirection = Character->GetActorRotation() + FRotator(0, 90, 0);
	}
	else if (ClosestSocket.IsEqual("LeftSocket"))
	{
		MovementDirection = GetActorForwardVector();
		FacingDirection = Character->GetActorRotation() + FRotator(0, -90, 0);
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, ClosestSocket.ToString());
	}
}


// Called every frame
void AMM_CoverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMM_CoverActor::RetrieveDirectionAndRotation(FVector & MovementDirection, FRotator & FacingRotation)
{
	DetermineMovementDirection(MovementDirection, FacingRotation);
}

