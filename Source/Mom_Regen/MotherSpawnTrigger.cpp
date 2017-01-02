// Fill out your copyright notice in the Description page of Project Settings.

#include "Mom_Regen.h"
#include "MotherSpawnTrigger.h"

#include "maze_globals.h"

using namespace MazeAlg;

// Sets default values
AMotherSpawnTrigger::AMotherSpawnTrigger() : spawn(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	loc = GetActorLocation();

	this->SetActorHiddenInGame(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> applyMesh(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	this->static_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	if (applyMesh.Object) {
		static_mesh->SetStaticMesh(applyMesh.Object);
	}
	int scale_factor = 25 * (PIECE_SIDE_LENGTH / 100);
	this->SetActorScale3D(FVector(scale_factor, scale_factor, 5));
	this->static_mesh->SetWorldScale3D(FVector(scale_factor, scale_factor, scale_factor));
	this->static_mesh->SetCollisionProfileName(TEXT("OverlapAll"));
}

// Called when the game starts or when spawned
void AMotherSpawnTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	
}

void AMotherSpawnTrigger::NotifyActorEndOverlap(AActor* OtherActor) {
	Super::NotifyActorEndOverlap(OtherActor);
	
	if (OtherActor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) && spawn != nullptr) {
		FActorSpawnParameters param;	// Sets up some parameter values for spawning
		param.Owner = this;
		param.Instigator = Instigator;
		*spawn = GetWorld()->SpawnActor<AMother>(AMother::StaticClass(), loc, FRotator(0, 0, 0), param);
		(*spawn)->setVectorLocation(loc);
		(*spawn)->setMazeLocation(start);
		Destroy();
	}
}

