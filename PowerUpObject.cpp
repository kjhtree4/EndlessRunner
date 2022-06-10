// Fill out your copyright notice in the Description page of Project Settings.

#include "BountyDashCharacter.h"
#include "PowerUpObject.h"

APowerUpObject::APowerUpObject() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);
	Mesh->AttachTo(RootComponent);
	Mesh->SetCollisionProfileName("OvelapAllDynamic");

	isSpawnAbove = false;
}
void APowerUpObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Mesh->AddLocalRotation(FRotator(0.0f, 0.0f, 5.0f));
	
}


void APowerUpObject::BeginPlay() {

	Super::BeginPlay();

	SetItemType();
}


void APowerUpObject::MyOnActorBeginOverlap(AActor * OverlappedAActor, AActor * otherActor)
{
	if (otherActor->GetClass()->IsChildOf(APowerUpObject::StaticClass())) {
		USphereComponent * thisSphere = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));

		USphereComponent* otherSphere = Cast<USphereComponent>(otherActor->GetComponentByClass(USphereComponent::StaticClass()));

		if (otherSphere && !(isSpawnAbove)) {
			AddActorLocalOffset(FVector(0.0f, 0.0f, (otherSphere->GetUnscaledSphereRadius()*2.0f) + Collider->GetUnscaledSphereRadius()));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("one item spawn above"));
			isSpawnAbove = true;
			// (otherSphere->GetUnscaledSphereRadius()*2.0f) + Collider->GetUnscaledSphereRadius() �� �� ����ؼ� �������� Ȯ���غ���
			//�̰� �ѹ��� ������ ����Ǽ� �� ���� ������ �� ����. ���� �ϳ� �����ؼ� �� �� �ø��� ī��Ʈ �����ְ� �ٽ� ȣ�� �ȵǵ��� �ϰ� �غ���.
		}
	}


	if (otherActor->GetClass()->IsChildOf(ABountyDashCharacter::StaticClass())) {
		ABountyDashCharacter *myChar = Cast<ABountyDashCharacter>(otherActor);
		
		if (IsValid(myChar)) {
	       myChar->PowerUp(Type);
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("myChar not valid"));
		}
	

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("You got Item!"));
		//�̺κ� enum���� ���� switch���ۼ��ؼ� ���� ������ ������� ����� �غ���

		switch (Type) {
		case EPowerUp::SPEED: {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("You got SPEED Item!"));
			break;
		}
		case EPowerUp::SMASH: {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("You got SMASH Item!"));
			break;
		}
		case EPowerUp::MAGNET: {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("You got MAGNET Item!"));
			break;
		}
		default:
			break;
		}

		Destroy(this);
	}

}

void APowerUpObject::MySetStaticMesh(EPowerUp StaticMeshTypeEnum)
{
	switch (StaticMeshTypeEnum) {
	case EPowerUp::SPEED: {
		Mesh->SetStaticMesh(SPEEDMesh);
		Mesh->SetMaterial(0,SPEEDMat);
		Mesh->SetCollisionProfileName("OvelapAllDynamic");
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("the SPEEDITEM mesh setted"));
		break;
	}
	case EPowerUp::SMASH: {
		Mesh->SetStaticMesh(SMASHMesh);
		Mesh->SetMaterial(0, SMASHMat);
		Mesh->SetCollisionProfileName("OvelapAllDynamic");
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("the SMASHITEM mesh setted"));
		break;
	}
	case EPowerUp::MAGNET: {
		Mesh->SetStaticMesh(MAGNETMesh);
		Mesh->SetMaterial(0, MAGNETMat);
		Mesh->SetCollisionProfileName("OvelapAllDynamic");
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("the MAGNETITEM mesh setted"));
		break;
	}
	default:
		break;
	}
}

void APowerUpObject::SetItemType()
{
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("TEST"));
	int iType = FMath::Rand() % 3;

	switch (iType)
	{
	case 0:
	{
		Type = EPowerUp::SPEED;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("the SPEEDITEM Selected"));
		break;
	}
	case 1:
	{
		Type = EPowerUp::SMASH;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("the SMASHITEM Selected"));
		break;
	}
	case 2:
	{
		Type = EPowerUp::MAGNET;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("the MAGNETITEM Selected"));
		break;
	}
	}
	MySetStaticMesh(Type);
}
