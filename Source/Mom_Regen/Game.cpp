// Fill out your copyright notice in the Description page of Project Settings.

#include "Mom_Regen.h"
#include "Game.h"

#include "maze_globals.h"

using namespace MazeAlg;

// Sets default values
AGame::AGame() : maze(nullptr), mother(nullptr), player(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void AGame::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	
	if (World) {
		FActorSpawnParameters param;	// Sets up some parameter values for spawning
		param.Owner = this;
		param.Instigator = Instigator;

		maze = World->SpawnActor<AMaze>(AMaze::StaticClass(), MazeAlg::START_VECTOR, FRotator(0, 0, 0), param);
		/* TODO : Determine Mother's Location for spawning. */
		player = UGameplayStatics::GetPlayerCharacter(World, 0);
		Location start = maze->getStart();
		player->SetActorLocation(determineLocation(start.r, start.c) + FVector(0, 0, 100));
		mother = World->SpawnActor<AMother>(AMother::StaticClass(), determineLocation(start.r, start.c), FRotator(0, 0, 0), param);
	}
}

// Called every frame
void AGame::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

