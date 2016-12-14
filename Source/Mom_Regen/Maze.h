// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "MazePiece.h"
#include "maze_globals.h"
#include <utility>

#include "GameFramework/Actor.h"
#include "Maze.generated.h"

UCLASS()
class MOM_REGEN_API AMaze : public AActor
{
	GENERATED_BODY()

private:
	TArray<TArray<char>> maze;
	TArray<TArray<AMazePiece*>> obj_maze;

	MazeAlg::Location start, end;
	
public:
	// Sets default values for this actor's properties
	AMaze();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Randomly regenerates maze
	bool regenerate(bool testMaze(const TArray<TArray<char>>&, std::pair<MazeAlg::Location, MazeAlg::Location>&));

	// Respawns maze in the game
	bool respawn();

	// Accessor for start of maze
	MazeAlg::Location getStart() const {
		return this->start;
	}

	// Accessor for end of maze
	MazeAlg::Location getEnd() const {
		return this->end;
	}

	// Accessor for maze by reference
	const TArray<TArray<char>>& getMaze() const {
		return this->maze;
	}

	


};

