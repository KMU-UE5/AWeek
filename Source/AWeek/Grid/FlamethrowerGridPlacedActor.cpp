// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/FlamethrowerGridPlacedActor.h"

#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"


AFlamethrowerGridPlacedActor::AFlamethrowerGridPlacedActor()
{
	YawPivot = CreateDefaultSubobject<USceneComponent>(TEXT("YawPivot"));
	YawPivot->SetupAttachment(Root);
	
	TurretHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(YawPivot);

	FlameFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FlameFX"));
	FlameFX->SetupAttachment(YawPivot);

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(YawPivot);

	DetectCapsule = CreateDefaultSubobject<USphereComponent>(TEXT("DetectCapsule"));
	DetectCapsule->SetupAttachment(Root);
	DetectCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AFlamethrowerGridPlacedActor::BeginPlay()
{
	Super::BeginPlay();
	if (YawPivot && GetWorld())
	{
		/*// 0.5초마다 TurretLineTrace 실행
		GetWorldTimerManager().SetTimer(
			TraceTimerHandle,
			this,
			&AFlamethrowerGridPlacedActor::TurretLineTrace,
			TraceInterval,
			true   // 반복
		);*/
	}
}

void AFlamethrowerGridPlacedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//지금 매 tick 마다 Overlap 확인하는 코드임
	//최적화 ㄱㅊ?
	TArray<AActor*> Overlaps;
	if (TargetClass)
		DetectCapsule->GetOverlappingActors(Overlaps, TargetClass);
	else
		DetectCapsule->GetOverlappingActors(Overlaps);

	if (Overlaps.Num() == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Turret overlap is none"));
		EndFlame();
		return;
	}
	StartFlame();
	UE_LOG(LogTemp, Log, TEXT("Turret overlap is %d"), Overlaps.Num());
	// 가장 가까운 타겟 찾기 -> TODO첫 타겟 계속 쫒아가기
	AActor* BestTarget = nullptr;
	float BestDistSq = TNumericLimits<float>::Max();
	const FVector MyLoc = YawPivot->GetComponentLocation();

	for (AActor* T : Overlaps)
	{
		if (!IsValid(T) || T == this) continue;
		float DistSq = FVector::DistSquared(MyLoc, T->GetActorLocation());
		if (DistSq < BestDistSq)
		{
			BestDistSq = DistSq;
			BestTarget = T;
		}
	}
	if (!BestTarget) return;

	FVector ToTarget = BestTarget->GetActorLocation() - MyLoc;
	if (bYawOnly) ToTarget.Z = 0.f;
	if (ToTarget.IsNearlyZero()) return;

	FRotator TargetRot = ToTarget.Rotation();
	FRotator CurrentRot = YawPivot->GetComponentRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, RotateSpeed);

	if (bYawOnly)
		NewRot.Pitch = CurrentRot.Pitch;

	YawPivot->SetWorldRotation(NewRot);

	UpdateFlame(DeltaTime);
}

void AFlamethrowerGridPlacedActor::TurretLineTrace()
{
	if (!YawPivot || !GetWorld())
	{
		return;
	}

	const FVector Start = YawPivot->GetComponentLocation();
	const FVector Forward = YawPivot->GetForwardVector(); // 포탑이 바라보는 방향
	const FVector End = Start + Forward * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(TurretTrace), false, this);
	Params.bReturnPhysicalMaterial = false;
	Params.AddIgnoredActor(this); // 자기 자신 무시

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		TraceChannel,
		Params
	);

	// 디버그 라인
	const FColor LineColor = bHit ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 0.5f, 0, 2.f);

	if (bHit && Hit.GetActor())
	{
		UE_LOG(LogTemp, Log, TEXT("[Turret Trace] Hit Actor: %s"), *Hit.GetActor()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[Turret Trace] No Hit"));
	}
}

void AFlamethrowerGridPlacedActor::UpdateFlame(float DeltaTime)
{
	float EffectiveLength = DoFlameTrace();

	// 2) 그 길이만큼 DamageBox 위치/크기 조정
	UpdateDamageVolume(EffectiveLength);

	// 3) 그 범위 안의 Actor에 데미지
	//ApplyFlameDamage();

	// 4) Niagara 이펙트 길이/파라미터 갱신
	//UpdateNiagaraFX(EffectiveLength);
}

float AFlamethrowerGridPlacedActor::DoFlameTrace() const
{
	if (!GetWorld() || !YawPivot) return 0.f;

	FVector Start = YawPivot->GetComponentLocation();
	FVector Dir   = YawPivot->GetForwardVector().GetSafeNormal();
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

void AFlamethrowerGridPlacedActor::UpdateDamageVolume(float EffectiveLength)
{
	if (!DamageBox || !YawPivot) return;

	FVector Start = YawPivot->GetComponentLocation();
	FVector Dir   = YawPivot->GetForwardVector().GetSafeNormal();

	float Width  = 80.f;
	float Height = 80.f;

	float HalfLen = EffectiveLength * 0.5f;

	DamageBox->SetBoxExtent(FVector(HalfLen, Width * 0.5f, Height * 0.5f));

	FVector Center = Start + Dir * HalfLen;
	FRotator Rot   = Dir.Rotation();

	DamageBox->SetWorldLocationAndRotation(Center, Rot);
	DamageBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AFlamethrowerGridPlacedActor::StartFlame()
{
	bIsFiring = true;
	/*if (FlameFX)
	{
		FlameFX->Activate();
		UE_LOG(LogTemp, Log, TEXT("A : %s"), *FlameFX->GetName());
		
	}*/

	if (FlameFX && !FlameFX->IsActive())
	{
		FlameFX->Activate(true);  // Reset = true

		UE_LOG(LogTemp, Log, TEXT("StartFlame : System=%s IsActive=%d IsVisible=%d"),
			*GetNameSafe(FlameFX->GetAsset()),
			FlameFX->IsActive(),
			FlameFX->IsVisible());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("StartFlame : FlameFX is nullptr!"));
	}
}
void AFlamethrowerGridPlacedActor::EndFlame()
{
	if (FlameFX && FlameFX->IsActive()) FlameFX->Deactivate();
	UE_LOG(LogTemp, Log, TEXT("B"));
	if (DamageBox) DamageBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}



