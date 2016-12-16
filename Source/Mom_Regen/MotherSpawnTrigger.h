// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Mother.h"
#include "maze_globals.h"

#include "GameFramework/Actor.h"
#include "MotherSpawnTrigger.generated.h"

UCLASS()
class MOM_REGEN_API AMotherSpawnTrigger : public AActor
{
	GENERATED_BODY()

private:
	AMother** spawn;
	FVector loc;
	MazeAlg::Location start;
	
	UStaticMeshComponent* static_mesh;
public:	
	// Sets default values for this actor's properties
	AMotherSpawnTrigger();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called when Actor no longer overlaps
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	// Will place address of spawned mother to specified pointer at specified location
	void spawnTo(AMother** spawn, const MazeAlg::Location& start, const FVector& offset) {
		this->spawn = spawn;
		this->start = start;
		this->loc = MazeAlg::determineLocation(start.r, start.c) + offset;
	}
	
	
};
