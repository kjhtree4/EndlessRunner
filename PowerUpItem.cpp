// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpItem.h"

APowerUpItem::APowerUpItem() {
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);
	Mesh->AttachTo(RootComponent);
	Mesh->SetCollisionProfileName("OvelapAllDynamic");

}

void APowerUpItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Mesh->AddLocalRotation(FRotator(5.0f, 0.0f, 0.0f));
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("tttt"));
}

void APowerUpItem::MyOnActorBeginOverlap(AActor * OverlappedAActor, AActor * otherActor)
{
}

void APowerUpItem::BeginPlay()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("tttt2"));
}

/*
void APowerUpItem::MySetStaticMesh(EPowerUp StaticMeshTypeEnum)
{
}

void APowerUpItem::SetItemType()
{
}
*/