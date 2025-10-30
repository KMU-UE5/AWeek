// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TurretPlacedActor.h"

#include "AWeek/Grid/TurretComponent.h"
#include "Components/SphereComponent.h"
#include "Components/ArrowComponent.h"

ATurretPlacedActor::ATurretPlacedActor()
{
	YawPivot = CreateDefaultSubobject<USceneComponent>(TEXT("YawPivot"));
	YawPivot->SetupAttachment(BoxMesh ? static_cast<USceneComponent*>(BoxMesh) : Root);

	PitchPivot = CreateDefaultSubobject<USceneComponent>(TEXT("PitchPivot"));
	PitchPivot->SetupAttachment(YawPivot);

	TurretHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(PitchPivot);
	TurretHead->SetGenerateOverlapEvents(false);
	TurretHead->SetSimulatePhysics(false);


	MuzzleArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleArrow"));
	MuzzleArrow->SetupAttachment(TurretHead);

	if (MuzzleArrow && TurretHead)
	{
		MuzzleArrow->AttachToComponent(TurretHead, FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("Muzzle"));
	}
	
	/*Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(TurretHead);*/

	TurretLogic = CreateDefaultSubobject<UTurretComponent>(TEXT("TurretLogic"));

	SenseSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SenseSphere"));
	SenseSphere->SetupAttachment(GetRootComponent()); //일단 root
	SenseSphere->InitSphereRadius(2000.f);

	SenseSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SenseSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	SenseSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SenseSphere->SetGenerateOverlapEvents(true);

	UE_LOG(LogTemp, Log, TEXT("Sense test1"));
	if (TurretLogic)
	{
		UE_LOG(LogTemp, Log, TEXT("Sense test2"));
		//TODO TurretComponent Setting
		TurretLogic->YawPivot = YawPivot;
		TurretLogic->PitchPivot = PitchPivot;
		TurretLogic->Muzzle = MuzzleArrow;

		//TODO TurretComponent Dmg, Bullet type, Rotate Speed Setting
	}
}

void ATurretPlacedActor::BeginPlay()
{
	Super::BeginPlay();
	SenseSphere->OnComponentBeginOverlap.AddDynamic(this, &ATurretPlacedActor::OnSenseBegin);
	SenseSphere->OnComponentEndOverlap.AddDynamic(this, &ATurretPlacedActor::OnSenseEnd);

	if (TurretLogic)
	{
		UE_LOG(LogTemp, Log, TEXT("Sense test2"));
		//TODO TurretComponent Setting
		TurretLogic->YawPivot = YawPivot;
		TurretLogic->PitchPivot = PitchPivot;
		TurretLogic->Muzzle = MuzzleArrow;

		//TODO TurretComponent Dmg, Bullet type, Rotate Speed Setting
	}
}


void ATurretPlacedActor::OnSenseBegin(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp,
									  int32 BodyIndex, bool bFromSweep, const FHitResult& Sweep)
{
	if (!Other || Other == this) return;
	if (APawn* Pawn = Cast<APawn>(Other))
	{
		//TODO TurretLogic add target and check zombie class
		TurretLogic->AddTarget(Pawn);
	}
}

void ATurretPlacedActor::OnSenseEnd(UPrimitiveComponent* Overlapped, AActor* Other, UPrimitiveComponent* OtherComp, int32 BodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("Sense test1"));
	if (!Other || Other == this) return;
	UE_LOG(LogTemp, Log, TEXT("Sense test2"));
	if (APawn* Pawn = Cast<APawn>(Other))
	{
		UE_LOG(LogTemp, Log, TEXT("Sense test3"));
		//TODO TurretLogic remove target and check zombie class
		TurretLogic->RemoveTarget(Pawn);
	}
}
