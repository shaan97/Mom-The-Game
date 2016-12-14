// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Maze.h"
#include "Mother.h"

#include "GameFramework/Actor.h"
#include "Game.generated.h"

UCLASS()
class MOM_REGEN_API AGame : public AActor
{
	GENERATED_BODY()

private:
	AMaze* maze;
	AMother* mother;
	AActor* player;

	long numTicks;		// Number of ticks since last update of maze.

	long ticksForUpdate;	// Number of ticks that pass before updating.

public:	
	// Sets default values for this actor's properties
	AGame();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};

