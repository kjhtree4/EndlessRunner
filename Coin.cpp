// Fill out your copyright notice in the Description page of Project Settings.

#include "BountyDashCharacter.h"
#include "Obstacle.h"
#include "BountyDashParticle.h"
#include "Coin.h"

ACoin::ACoin() {
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	check(Mesh);
	Mesh->AttachTo(RootComponent);
	Mesh->SetCollisionProfileName("OvelapAllDynamic");

	isSpawnAbove = false;
	test = 1;
}

void ACoin::BeginPlay()
{
	Super::BeginPlay();

}


void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Mesh->AddLocalRotation(FRotator(5.0f, 0.0f, 0.0f));
}

void ACoin::MyOnActorBeginOverlap(AActor *OverlappedAActor, AActor *otherActor)//만약 스폰했는데 다른 옵스타클과 겹치면 위로 올려버리기
{
	if (otherActor->GetClass()->IsChildOf(AObstacle::StaticClass())&& !BeingPulled) {
		USphereComponent * thisSphere = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));

		USphereComponent* otherSphere= Cast<USphereComponent>(otherActor->GetComponentByClass(USphereComponent::StaticClass()));

		if (otherSphere&& !(isSpawnAbove)&&test==1) {
			isSpawnAbove = true;//부울말고 인트로해서 2번씩적용되는게 있는지 확인하자
			AddActorLocalOffset(FVector(0.0f, 0.0f, (otherSphere->GetUnscaledSphereRadius()*2.0f) + Collider->GetUnscaledSphereRadius()));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("one coin spawn above"));
			test++;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), (otherSphere->GetUnscaledSphereRadius()*2.0f) + Collider->GetUnscaledSphereRadius()));
			if (test > 2) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), test));
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("dfgdfgdfggfg")));
			}

			// (otherSphere->GetUnscaledSphereRadius()*2.0f) + Collider->GetUnscaledSphereRadius() 이 값 출력해서 동일한지 확인해보기
			//이거 한번에 여러번 적용되서 저 위로 나오는 거 같음. 변수 하나 생성해서 한 번 올리면 카운트 시켜주고 다시 호출 안되도록 하게 해보자.
		}
	}

	if (otherActor->GetClass()->IsChildOf(ABountyDashCharacter::StaticClass())) {
		ABountyDashParticle *particleSys = GetWorld()->SpawnActor<ABountyDashParticle>(ABountyDashParticle::StaticClass(), GetTransform());
		particleSys->SetKillPoint(GetKillPoint());


		ABountyDashCharacter *myChar = Cast<ABountyDashCharacter>(otherActor);
		myChar->ScoreUp();
		Destroy(this);

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("you got coin!"));
	}
}
