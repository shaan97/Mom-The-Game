// Fill out your copyright notice in the Description page of Project Settings.

#include "Mom_Regen.h"
#include "Game.h"

#include "maze_globals.h"

using namespace MazeAlg;

// Sets default values
AGame::AGame() : maze(nullptr), mother(nullptr), player(nullptr), trigger(nullptr), numTicks(0), ticksForUpdate(180)
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

		player = UGameplayStatics::GetPlayerCharacter(World, 0);
		Location start = maze->getStart();
		FVector vec_loc = determineLocation(start.r, start.c);
		player->SetActorLocation(vec_loc + FVector(0, 0, 100));
		
		trigger = World->SpawnActor<AMotherSpawnTrigger>(AMotherSpawnTrigger::StaticClass(), vec_loc + FVector(0, 0, 100), FRotator(0, 0, 0), param);
		trigger->spawnTo(&mother, start, FVector(0, 0, 300));

		/* TODO : Determine Mother's Location for spawning. Make sure to set Mother's internal state appropriately.
				  Specifically, send in the location into Mother::setMazeLocation(const Location& maze_location);
		*/
		/*
		FVector vec_loc = determineLocation(start.r, start.c) + FVector(0, 0, 300);
		mother = World->SpawnActor<AMother>(AMother::StaticClass(), vec_loc, FRotator(0, 0, 0), param);
		mother->setVectorLocation(vec_loc);	
		mother->setMazeLocation(start);		// NEEDS TO BE CHANGED
		*/
	}
}

// Called every frame
void AGame::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (numTicks++ == ticksForUpdate && mother != nullptr) {
		this->mother->update_dir(maze->getMaze(), convert(player->GetActorLocation()));
		numTicks = 0;
	}
}

