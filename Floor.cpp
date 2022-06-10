// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/BoxComponent.h"
#include "BountyDashGameModeBase.h"
#include "DestroyedFloorPiece.h"
#include "Floor.h"

// Sets default values
AFloor::AFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ConstructorHelpers::FObjectFinder<UStaticMesh>myMesh(TEXT("/Game/Geometry/Floor_Mesh_BountyDash.Floor_Mesh_BountyDash"));
	//ConstructorHelpers::FObjectFinder<UMaterial>myMaterial(TEXT("/Game/StarterContent/Materials/M_Concrete_Tiles.M_Concrete_Tiles"));


	//ABountyDashGameModeBase *MyGameMode = Cast<ABountyDashGameModeBase>(GetWorld()->GetAuthGameMode());
	//auto *MyGameMode = Cast<ABountyDashGameModeBase>(GetWorld()->GetAuthGameMode());
	//위의 명령어들은 서버에서 실행할 수 있는 명령어들이
	/*
	if (IsValid(MyGameMode)) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("GetGameMode Success"));
	}
	else{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("GetGameMode failed"));
	}
	*/

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	ConstructorHelpers::FObjectFinder<UStaticMesh>myMesh(TEXT("/Game/Geometry/Destroyable_Mesh.Destroyable_Mesh"));
	ConstructorHelpers::FObjectFinder<UMaterial>myMaterial(TEXT("/Game/StarterContent/Materials/M_Concrete_Tiles.M_Concrete_Tiles"));

	
	if (myMesh.Succeeded()) {
		NumRepeatingMesh = 80;

		FBoxSphereBounds myBounds = myMesh.Object->GetBounds();
		float XBounds = myBounds.BoxExtent.X * 2;//바닥 x축 길이
		float ScenePos = ((XBounds*(NumRepeatingMesh - 1)) / 2.0f)*-1;

		KillPoint = ScenePos - (XBounds *0.5f);
		SpawnPoint = (ScenePos*-1) + (XBounds * 0.5f);

		for (int i = 0; i < NumRepeatingMesh; ++i) {
			FString SceneName = "Scene" + FString::FromInt(i);
			FName SceneID = FName(*SceneName);
			USceneComponent *thisScene = CreateDefaultSubobject<USceneComponent>(SceneID);

			check(thisScene);
			thisScene->AttachTo(RootComponent);
			thisScene->SetRelativeLocation(FVector(ScenePos, 0.0f, 0.0f));
			ScenePos += XBounds;

			FloorMeshScene.Add(thisScene);

			FString MeshName = "Mesh" + FString::FromInt(i);

			UStaticMeshComponent *thisMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName(*MeshName));
			check(thisMesh);

			thisMesh->AttachTo(FloorMeshScene[i]);
			thisMesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0f));
			thisMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

			if (myMaterial.Succeeded()) {
				thisMesh->SetStaticMesh(myMesh.Object);
				thisMesh->SetMaterial(0, myMaterial.Object);
			}
			FloorMeshes.Add(thisMesh);
		}

		CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
		check(CollisionBox);

		CollisionBox->AttachTo(RootComponent);
		CollisionBox->SetBoxExtent(FVector(SpawnPoint, myBounds.BoxExtent.Y, myBounds.BoxExtent.Z));

		CollisionBox->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	}
	
}

float AFloor::GetKillPoint()
{
	return KillPoint;
}

float AFloor::GetSpawnPoint()
{
	return SpawnPoint;
}

// Called when the game starts or when spawned
void AFloor::BeginPlay()
{
	Super::BeginPlay();

	MyGameMode = Cast<ABountyDashGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())); //겟 게임모드는 생성자에 넣으니까 겟하지를 못하더라. 그래서 비긴플레이에 넣음

	if (IsValid(MyGameMode)) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("GetGameMode Success"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("GetGameMode failed"));
	}
	
}

// Called every frame
//이 부분 분석해야 함
void AFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		for (auto Scene : FloorMeshScene) {
			Scene->AddLocalOffset(FVector(MyGameMode->GetInvGameSpeed(), 0.0f, 0.0f));

			if (Scene->GetComponentTransform().GetLocation().X <= KillPoint) {

				ADestroyedFloorPiece *thisPiece = GetWorld()->SpawnActor<ADestroyedFloorPiece>(ADestroyedFloorPiece::StaticClass(), Scene->GetComponentTransform());

				if (thisPiece) {
					thisPiece->Destructable->ApplyDamage(100000, thisPiece->GetActorLocation(), FVector(-FMath::RandRange(-10, 10), -FMath::RandRange(-10, 10), -FMath::RandRange(-10, 10)), 10000);
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("FloorBoxDestruct"));
				}
				else {

				}
				Scene->SetRelativeLocation(FVector(SpawnPoint, 0.0f, 0.0f));
			}
		}
}


