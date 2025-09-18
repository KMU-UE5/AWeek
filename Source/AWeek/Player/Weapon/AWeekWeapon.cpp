// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekWeapon.h"

// Sets default values
AAWeekWeapon::AAWeekWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SetRootComponent(mMeshComponent);

	mMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AAWeekWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAWeekWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

