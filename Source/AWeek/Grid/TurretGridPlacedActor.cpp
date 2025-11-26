// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TurretGridPlacedActor.h"

#include "Components/SphereComponent.h"
#include "TimerManager.h"

ATurretGridPlacedActor::ATurretGridPlacedActor()
{
	PrimaryActorTick.bCanEverTick = true;


	YawPivot = CreateDefaultSubobject<USceneComponent>(TEXT("YawPivot"));
	YawPivot->SetupAttachment(Root);
	
	TurretHead = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretHead"));
	TurretHead->SetupAttachment(YawPivot);

	DetectCapsule = CreateDefaultSubobject<USphereComponent>(TEXT("DetectCapsule"));
	DetectCapsule->SetupAttachment(Root);
	DetectCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectCapsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ATurretGridPlacedActor::BeginPlay()
{
	Super::BeginPlay();
	if (YawPivot && GetWorld())
	{
		// 0.5초마다 TurretLineTrace 실행
		GetWorldTimerManager().SetTimer(
			TraceTimerHandle,
			this,
			&ATurretGridPlacedActor::TurretLineTrace,
			TraceInterval,
			true   // 반복
		);
	}
}


void ATurretGridPlacedActor::Tick(float DeltaTime)
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
		return;
	} 
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
}

void ATurretGridPlacedActor::TurretLineTrace()
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
