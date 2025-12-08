// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TestFlamethrowerActor.h"

#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// Sets default values
ATestFlamethrowerActor::ATestFlamethrowerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	Muzzle = CreateDefaultSubobject<USceneComponent>("Muzzle");
	Muzzle->SetupAttachment(Root);
	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(Root);
	DamageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	DamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	FlameFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlameFX"));
	FlameFX->SetupAttachment(Muzzle);



	FlameFX->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ATestFlamethrowerActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log,TEXT("b"));
	if (FlameFX && FlameFXSystem)
	{
		UE_LOG(LogTemp, Log,TEXT("a"));
		FlameFX->SetAsset(FlameFXSystem);
	}

	StartFlame();
}

// Called every frame
void ATestFlamethrowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsFiring)
	{
		UpdateFlame(DeltaTime);
	}
}

void ATestFlamethrowerActor::StartFlame()
{
	bIsFiring = true;
	if (FlameFX) FlameFX->Activate();
}

void ATestFlamethrowerActor::EndFlame()
{
	bIsFiring = false;
	if (FlameFX) FlameFX->Deactivate();
	if (DamageBox) DamageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATestFlamethrowerActor::UpdateFlame(float DeltaTime)
{
	float EffectiveLength = DoFlameTrace();

	// 2) 그 길이만큼 DamageBox 위치/크기 조정
	UpdateDamageVolume(EffectiveLength);

	// 3) 그 범위 안의 Actor에 데미지
	ApplyFlameDamage();

	// 4) Niagara 이펙트 길이/파라미터 갱신
	UpdateNiagaraFX(EffectiveLength);
}

float ATestFlamethrowerActor::DoFlameTrace() const
{
	if (!GetWorld() || !Muzzle) return 0.f;

	FVector Start = Muzzle->GetComponentLocation();
	FVector Dir   = Muzzle->GetForwardVector().GetSafeNormal();
	FVector End   = Start + Dir * MaxRange;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(FlameTrace), false, this);

	// Sphere 또는 Capsule 추천 (폭 표현용)
	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,                         // 장애물 채널
		FCollisionShape::MakeSphere(20.f),      // 화염 반지름
		Params
	);

	if (bHit)
	{
		return Hit.Distance;    // 장애물까지 거리
	}

	return MaxRange;            // 막히지 않으면 최대 거리
}

void ATestFlamethrowerActor::UpdateDamageVolume(float EffectiveLength)
{
	if (!DamageBox || !Muzzle) return;

	FVector Start = Muzzle->GetComponentLocation();
	FVector Dir   = Muzzle->GetForwardVector().GetSafeNormal();

	float Width  = 80.f;
	float Height = 80.f;

	float HalfLen = EffectiveLength * 0.5f;

	DamageBox->SetBoxExtent(FVector(HalfLen, Width * 0.5f, Height * 0.5f));

	FVector Center = Start + Dir * HalfLen;
	FRotator Rot   = Dir.Rotation();

	DamageBox->SetWorldLocationAndRotation(Center, Rot);
	DamageBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ATestFlamethrowerActor::ApplyFlameDamage()
{
	if (!DamageBox) return;


	TArray<AActor*> OverlappingActors;
	DamageBox->GetOverlappingActors(OverlappingActors); 
	
	for (AActor* Other : OverlappingActors)
	{
		if (!Other || Other == this) 
		{
			continue;
		}

		UE_LOG(LogTemp, Log, TEXT("DamageBox Overlap Actor: %s"), *Other->GetName());

	}
}

void ATestFlamethrowerActor::UpdateNiagaraFX(float EffectiveLength)
{
	
}







