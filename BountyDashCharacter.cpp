// Fill out your copyright notice in the Description page of Project Settings.

//#include "BountyDashCharacter.h"
#include "Components/AudioComponent.h"
#include "Coin.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimBlueprint.h"
#include "BountyDashGameModeBase.h"
#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "Engine/Engine.h"
#include "Obstacle.h"
#include "Floor.h"
#include "Sound/SoundCue.h"
#include "BountyDashCharacter.h"


// Sets default values
ABountyDashCharacter::ABountyDashCharacter()
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	ConstructorHelpers::FObjectFinder<UAnimBlueprint>myAnimBP(TEXT("/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh>myMesh(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	if (myMesh.Succeeded() && myAnimBP.Succeeded()) {
		GetMesh()->SetSkeletalMesh(myMesh.Object);
		GetMesh()->SetAnimInstanceClass(myAnimBP.Object->GeneratedClass);
	}

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));//�޽��� ĸ���� �� ũ�⸸ŭ ����
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	GetCharacterMovement()->JumpZVelocity = 1450.0;
	GetCharacterMovement()->GravityScale = 4.5;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//check(CameraBoom);//���� ���ٸ� ��Ÿ�ӽ� ��� ����
	CameraBoom->AttachTo(RootComponent);//ĳ����Ŭ������ ��Ʈ������Ʈ�� ĸ��������Ʈ
	CameraBoom->TargetArmLength = 500.0f;//�� ���� ����
	CameraBoom->AddRelativeLocation(FVector(0.0f, 0.0f, 160.0f));


	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//check(FollowCamera);
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);//�̰� ���ϳ��� �̷����ϸ� ���ɵǴ°��� �𸣰���///////////////////////////////////////
	FollowCamera->AddRelativeRotation(FQuat(FRotator(-10.0f, 0.0f, 0.0f)));//�ణ ������ �� �� �ֵ��� ī�޶� ȸ�� ����

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABountyDashCharacter::MyOnComponentOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABountyDashCharacter::MyOnComponentEndOverlap);

	CharSpeed = 10.0f;

	CanSmash = false;
	CanMagnet = false;

	hitObstacleSound = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSound"));
	hitObstacleSound->bAutoActivate = false;
	hitObstacleSound->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<USoundCue>RockDestroyCue(TEXT("/Game/RockDestroyCue.RockDestroyCue"));

	if (RockDestroyCue.Succeeded()) {
		hitObstacleSound->SetSound(RockDestroyCue.Object);
	}

	dingSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Ding"));
	dingSound->bAutoActivate=false;
	dingSound->AttachTo(RootComponent);

	ConstructorHelpers::FObjectFinder<USoundCue>CoinCue(TEXT("/Game/CoinCue.CoinCue"));

	if (CoinCue.Succeeded()) {
		dingSound->SetSound(CoinCue.Object);
	}
}

// Called when the game starts or when spawned
void ABountyDashCharacter::BeginPlay()
{
	Super::BeginPlay();

	MyGameMode = Cast<ABountyDashGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	for (TActorIterator<ATargetPoint> TargetIter(GetWorld()); TargetIter; ++TargetIter) {
		TargetArray.Add(*TargetIter);
	}

	auto SortPred = [](const AActor& A, const AActor& B)->bool {
		return(A.GetActorLocation().Y < B.GetActorLocation().Y);
	};
	TargetArray.Sort(SortPred);

	CurrentLocation = ((TargetArray.Num() / 2) + (TargetArray.Num() % 2) - 1);  //Ȧ���� �� �߾ӿ� ��ġ�ϰ��ϰ� ¦���� ���߾��� ������ �߾Ӻ��� ���ڸ� ���� �� ä��


	for (TActorIterator<AFloor> TargetIter(GetWorld()); TargetIter; ++TargetIter) {
		Killpoint = TargetIter->GetKillPoint();
	}


}

void ABountyDashCharacter::MoveRight()
{

	if ((Controller != nullptr)) {							//��Ʈ�ѷ��� null�� �ƴ϶��, Controller�� Pawn���� �̸� ������ �Ǿ��ִ� �����ΰ� ����
		if ((CurrentLocation < TargetArray.Num() - 1)) {	//������ �������ڸ��� �ƴ϶��
			++CurrentLocation;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("MoveRight"));
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("CantmoveRight"));
		}
	}

}

void ABountyDashCharacter::MoveLeft()
{

	if ((Controller != nullptr)) {
		if (CurrentLocation > 0) { //ù��° �ڸ��� �ƴ϶��
			--CurrentLocation;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("moveLeft"));
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("CantmoveLeft"));
		}
	}

}

void ABountyDashCharacter::MyOnComponentOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->GetClass()->IsChildOf(AObstacle::StaticClass())) {
		FVector vecBetween = OtherActor->GetActorLocation() - GetActorLocation();//��ֹ��� �÷��̾� �ε����� ������ ����
		float AngleBetween = FMath::Acos(FVector::DotProduct(vecBetween.GetSafeNormal(), GetActorForwardVector().GetSafeNormal()));//���� ���Ϳ� �÷��̾� �����庤�͸� �����Ͽ� �÷��̾ ��ֹ��� �ٶ󺸰� �־����� �˻��ϱ�����.
		

		AngleBetween *= (180 / PI);
		if (AngleBetween < 60.0f) {
			AObstacle *pObs = Cast<AObstacle>(OtherActor);
			if (pObs && CanSmash) {
				pObs->GetDestructable()->ApplyRadiusDamage(10000, GetActorLocation(), 10000,10000, true);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Smash obstacle"));
			}
			else {
				bBeingPushed = true;
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Smash obstacle"));
			}	
			if (!bBeingPushed) {
				hitObstacleSound->Play();
			}
			
			
		}
	}
}

void ABountyDashCharacter::MyOnComponentEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->IsChildOf(AObstacle::StaticClass())) {
		bBeingPushed = false;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("colide out with obstacle"));
	}
}

void ABountyDashCharacter::StopSmash()
{
	CanSmash = false;
}

void ABountyDashCharacter::StopMagnet()
{
	CanMagnet = false;
}

void ABountyDashCharacter::CoinMagnet()
{
	//�ű׳� ��ġ Ȯ������

	for (TActorIterator<ACoin> coinIter(GetWorld()); coinIter; ++coinIter) {
		FVector between = GetActorLocation() - coinIter->GetActorLocation();

		if (FMath::Abs(between.Size()) < MagnetReach) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("MAgnetttt"));
			FVector CoinPos = FMath::Lerp((*coinIter)->GetActorLocation(), GetActorLocation(), 0.2f);
			(*coinIter)->SetActorLocation(CoinPos);
			(*coinIter)->BeingPulled = true;
			//coinIter->SetActorLocation(CoinPos);
		}
	}
}

// Called every frame
void ABountyDashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetArray.Num() > 0) {
		FVector TargetLoc = TargetArray[CurrentLocation]->GetActorLocation();//�÷��̾ �������� �� ��ǥ ����
		TargetLoc.Z = GetActorLocation().Z; //Y�ุ ���������ϴϱ� Z,X���� ������ ������ ������
		TargetLoc.X = GetActorLocation().X;

		if (TargetLoc != GetActorLocation()) {
			SetActorLocation(FMath::Lerp(GetActorLocation(), TargetLoc, CharSpeed *DeltaTime));
		}
	}

	if (bBeingPushed) {
		float moveSpeed = MyGameMode->GetInvGameSpeed();
		AddActorLocalOffset(FVector(moveSpeed, 0.0f, 0.0f));

		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("collider"));
	}
	/* �̰Ŵ� ƽ�Լ����� ��������Ʈ�� �� ȿ���� �����ϱ� ��������Ʈ�� ���߿� �ٲ㺸��*/
	if (CanMagnet) {
		CoinMagnet();
	}

	if (GetActorLocation().X < Killpoint) {
		MyGameMode->GameOver();
		
	}

}

// Called to bind functionality to input
void ABountyDashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &ABountyDashCharacter::MoveRight);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &ABountyDashCharacter::MoveLeft);
	InputComponent->BindAction("Reset", IE_Pressed, this, &ABountyDashCharacter::Reset).bExecuteWhenPaused = true;//������ ������ ���¿����� ��ǲ�׼��� �����ϰ� ���ִ� �������
}

void ABountyDashCharacter::ScoreUp()
{
	Score++;
	MyGameMode->CharScoreUp(Score);
	dingSound->Play();
}


void ABountyDashCharacter::PowerUp(EPowerUp Type) {
	switch (Type) {
		case EPowerUp::SPEED:{
			MyGameMode->ReduceGameSpeed();
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Reduce Game Speed Activate"));
			break;
		}
		case EPowerUp::SMASH: {
			CanSmash = true;
			FTimerHandle newTimer;
			GetWorld()->GetTimerManager().SetTimer(newTimer, this, &ABountyDashCharacter::StopSmash, SmashTime, false);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Smash Activate"));
			break;
		}
		case EPowerUp::MAGNET: {
			CanMagnet = true;
			FTimerHandle newTimer;
			GetWorld()->GetTimerManager().SetTimer(newTimer, this, &ABountyDashCharacter::StopMagnet, MagnetTime, false);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Magnet Activate"));
			break;
		}
		default:
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("default Activate"));
			break;
	}
}

int ABountyDashCharacter::GetScore() {
	return Score;
}

void ABountyDashCharacter::Reset() {
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("BountyDashMap"));
}