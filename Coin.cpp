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

void ACoin::MyOnActorBeginOverlap(AActor *OverlappedAActor, AActor *otherActor)//���� �����ߴµ� �ٸ� �ɽ�ŸŬ�� ��ġ�� ���� �÷�������
{
	if (otherActor->GetClass()->IsChildOf(AObstacle::StaticClass())&& !BeingPulled) {
		USphereComponent * thisSphere = Cast<USphereComponent>(GetComponentByClass(USphereComponent::StaticClass()));

		USphereComponent* otherSphere= Cast<USphereComponent>(otherActor->GetComponentByClass(USphereComponent::StaticClass()));

		if (otherSphere&& !(isSpawnAbove)&&test==1) {
			isSpawnAbove = true;//�ο︻�� ��Ʈ���ؼ� 2��������Ǵ°� �ִ��� Ȯ������
			AddActorLocalOffset(FVector(0.0f, 0.0f, (otherSphere->GetUnscaledSphereRadius()*2.0f) + Collider->GetUnscaledSphereRadius()));
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("one coin spawn above"));
			test++;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), (otherSphere->GetUnscaledSphereRadius()*2.0f) + Collider->GetUnscaledSphereRadius()));
			if (test > 2) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), test));
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("dfgdfgdfggfg")));
			}

			// (otherSphere->GetUnscaledSphereRadius()*2.0f) + Collider->GetUnscaledSphereRadius() �� �� ����ؼ� �������� Ȯ���غ���
			//�̰� �ѹ��� ������ ����Ǽ� �� ���� ������ �� ����. ���� �ϳ� �����ؼ� �� �� �ø��� ī��Ʈ �����ְ� �ٽ� ȣ�� �ȵǵ��� �ϰ� �غ���.
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
