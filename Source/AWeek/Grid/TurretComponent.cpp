// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeek/Grid/TurretComponent.h"

#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"

// Sets default values for this component's properties
UTurretComponent::UTurretComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTurretComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTurretComponent::OnRegister()
{
	Super::OnRegister();
	
}


// Called every frame
void UTurretComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	UpdateAim(DeltaTime);
}

void UTurretComponent::SetTargets()
{
	if (Targets.Num() == 0){bIsActive = false;}
	else if (CurrentTarget == nullptr)
	{
		bIsActive = true;
		CurrentTarget = Targets[0];
	}
}


void UTurretComponent::AddTarget(APawn* Target)
{
	Targets.Add(Target);
	SetTargets();
}

void UTurretComponent::RemoveTarget(APawn* Target)
{
	Targets.Remove(Target);
	if (CurrentTarget == nullptr)
	{
		SetTargets();
	}
	if (CurrentTarget == Target)
	{
		CurrentTarget = nullptr;
	}
}

void UTurretComponent::UpdateAim(float DT)
{
	/*//UE_LOG(LogTemp, Log, TEXT("Sense test1"));
	if (!YawPivot.IsValid() || !PitchPivot.IsValid()) return;
	const FVector AimFrom = Muzzle.IsValid()? Muzzle->GetComponentLocation() : PitchPivot->GetComponentLocation();
	
	FVector AimDir;
	if (CurrentTarget == nullptr)return;
	if (AActor* T = CurrentTarget.Get())
	{
		AimDir = (T->GetActorLocation() - AimFrom).GetSafeNormal();
	}else
	{
		AimDir = (Muzzle.IsValid()? Muzzle->GetForwardVector() : YawPivot->GetForwardVector());
	}

	//const FRotator Desired = AimDir.Rotation();

	/#1#/ --- Yaw: 수평 회전만 반영 ---
	FRotator YawRot = YawPivot->GetComponentRotation();
	YawRot.Yaw = FMath::FixedTurn(YawRot.Yaw, Desired.Yaw, YawSpeedDegPerSec * DT);
	YawPivot->SetWorldRotation(FRotator(0.f, YawRot.Yaw, 0.f));

	// --- Pitch: 상하 각도만 반영 ---
	FRotator PitchRot = PitchPivot->GetComponentRotation();
	const float NextPitch = FMath::Clamp(
		FMath::FixedTurn(PitchRot.Pitch, Desired.Pitch, PitchSpeedDegPerSec * DT),
		MinPitch, MaxPitch
	);
	PitchPivot->SetWorldRotation(FRotator(NextPitch, PitchPivot->GetComponentRotation().Yaw, 0.f));#1#








	const FRotator Desired = AimDir.Rotation();

	const float CurYawRaw   = YawPivot->GetComponentRotation().Yaw;
	const float CurPitchRaw = PitchPivot->GetComponentRotation().Pitch;
	const float CurYaw      = FMath::UnwindDegrees(CurYawRaw);
	const float CurPitch    = FMath::UnwindDegrees(CurPitchRaw);

	const float DesYaw   = FMath::UnwindDegrees(Desired.Yaw);
	const float DesPitch = FMath::UnwindDegrees(Desired.Pitch);

	// --- Yaw ---
	const float YawStepDeg = YawSpeedDegPerSec * DT;
	const float NextYaw    = FMath::FixedTurn(CurYaw, DesYaw, YawStepDeg);

	// 적용
	YawPivot->SetWorldRotation(FRotator(0.f, NextYaw, 0.f));

	// 로그 (Yaw)
	UE_LOG(LogTemp, Log,
		TEXT("[Turret][Yaw] cur=%.2f -> next=%.2f (des=%.2f, step<=%.2f, delta=%.2f) target=%s"),
		CurYaw, NextYaw, DesYaw, YawStepDeg,
		FMath::FindDeltaAngleDegrees(CurYaw, DesYaw),
		CurrentTarget.IsValid() ? *CurrentTarget->GetName() : TEXT("None"));

	// --- Pitch ---
	const float PitchStepDeg = PitchSpeedDegPerSec * DT;
	const float NextPitchRaw = FMath::FixedTurn(CurPitch, DesPitch, PitchStepDeg);
	const float NextPitch_    = FMath::Clamp(NextPitchRaw, MinPitch, MaxPitch);

	// 적용
	PitchPivot->SetWorldRotation(FRotator(NextPitch_, PitchPivot->GetComponentRotation().Yaw, 0.f));

	// 로그 (Pitch)
	UE_LOG(LogTemp, Log,
		TEXT("[Turret][Pitch] cur=%.2f -> next=%.2f (des=%.2f, step<=%.2f, delta=%.2f, clamp=[%.2f..%.2f]) target=%s"),
		CurPitch, NextPitch_, DesPitch, PitchStepDeg,
		FMath::FindDeltaAngleDegrees(CurPitch, DesPitch),
		MinPitch, MaxPitch,
		CurrentTarget.IsValid() ? *CurrentTarget->GetName() : TEXT("None"));*/

	// 필수 피벗은 없으면 종료
	USceneComponent* Yaw   = YawPivot.Get();
	USceneComponent* Pitch = PitchPivot.Get();
	if (!Yaw || !Pitch) return;

	// Muzzle(Arrow)가 있으면 그 위치/방향을 쓰고, 없으면 피치/요 피벗으로 대체
	const UArrowComponent* Mzl = Muzzle.Get();
	const FVector AimFrom = (Mzl ? Mzl->GetComponentLocation() : Pitch->GetComponentLocation());

	// --- 목표 방향 계산 ---
	FVector AimDir = FVector::ZeroVector;

	if (AActor* T = CurrentTarget.Get())
	{
		AimDir = (T->GetActorLocation() - AimFrom);
	}
	// 타깃이 없을 때도 멈추지 말고, 현재 전방을 유지
	if (AimDir.IsNearlyZero())
	{
		// Muzzle가 있으면 Muzzle 전방(+X), 없으면 YawPivot 전방
		AimDir = (Mzl ? Mzl->GetForwardVector() : Yaw->GetForwardVector());
	}
	AimDir = AimDir.GetSafeNormal();
	if (AimDir.IsNearlyZero()) return; // 그래도 0이면 이번 틱 스킵

	// --- 원하는 회전 ---
	const FRotator Desired = AimDir.Rotation();

	// 현재 각도(정규화)
	const float CurYaw   = FMath::UnwindDegrees(Yaw->GetComponentRotation().Yaw);
	const float CurPitch = FMath::UnwindDegrees(Pitch->GetComponentRotation().Pitch);
	const float DesYaw   = FMath::UnwindDegrees(Desired.Yaw);
	const float DesPitch = FMath::UnwindDegrees(Desired.Pitch);

	// --- Yaw ---
	const float YawStepDeg = YawSpeedDegPerSec * DT;
	const float NextYaw    = FMath::FixedTurn(CurYaw, DesYaw, YawStepDeg);
	Yaw->SetWorldRotation(FRotator(0.f, NextYaw, 0.f));

	UE_LOG(LogTemp, Log,
		TEXT("[Turret][Yaw] cur=%.2f -> next=%.2f (des=%.2f, step=%.2f, delta=%.2f) target=%s"),
		CurYaw, NextYaw, DesYaw, YawStepDeg,
		FMath::FindDeltaAngleDegrees(CurYaw, DesYaw),
		CurrentTarget.IsValid() ? *CurrentTarget->GetName() : TEXT("None"));

	// --- Pitch ---
	const float PitchStepDeg = PitchSpeedDegPerSec * DT;
	const float NextPitchRaw = FMath::FixedTurn(CurPitch, DesPitch, PitchStepDeg);
	const float NextPitch    = FMath::Clamp(NextPitchRaw, MinPitch, MaxPitch);

	// Yaw는 YawPivot이 책임지고 있으니, PitchPivot은 Pitch만 변경
	Pitch->SetWorldRotation(FRotator(NextPitch, Pitch->GetComponentRotation().Yaw, 0.f));

	UE_LOG(LogTemp, Log,
		TEXT("[Turret][Pitch] cur=%.2f -> next=%.2f (des=%.2f, step=%.2f, delta=%.2f, clamp=[%.2f..%.2f]) target=%s"),
		CurPitch, NextPitch, DesPitch, PitchStepDeg,
		FMath::FindDeltaAngleDegrees(CurPitch, DesPitch),
		MinPitch, MaxPitch,
		CurrentTarget.IsValid() ? *CurrentTarget->GetName() : TEXT("None"));
}
bool UTurretComponent::IsValidTarget(const APawn* A) const
{
	return false;
}


