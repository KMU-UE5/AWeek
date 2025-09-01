// Fill out your copyright notice in the Description page of Project Settings.


#include "AWeekWeapon.h"

// Sets default values
AAWeekWeapon::AAWeekWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(mMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/ThirdParty/mp-44-assault-rifle/source/MP441.MP441'"));
	
	if (MeshAsset.Succeeded())
		mMesh->SetStaticMesh(MeshAsset.Object);

	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

