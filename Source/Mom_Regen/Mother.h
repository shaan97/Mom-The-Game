// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "maze_globals.h"
#include <queue>

#include "GameFramework/Actor.h"
#include "Mother.generated.h"

UCLASS()
class MOM_REGEN_API AMother : public AActor
{
	GENERATED_BODY()
private:
	FVector vect_location;		// Mother's (x,y,z) location in space
	MazeAlg::Location maze_location;		// Mother's (r,c) location in the maze

	

	UPROPERTY(EditAnywhere)
	float acceleration;		// Acceleration at which mother travels (m/s/s)

	long numTicks;				// Number of ticks since last acceleration

	std::queue<FVector> dir;	// FVector directions from Mother to Daughter

	enum attack_type { FLOOR, LEFT_WALL, CEILING, RIGHT_WALL };
	attack_type approach;		// Mother's style of approach to sneak up on Player

	struct Camera {
		unsigned long left_wall, right_wall, ceiling;
	} cam;

	

	// Returns queue of Locations for shortest path from Mother to Player
	std::queue<MazeAlg::Location> shortestPath(TArray<TArray<char>> maze, const MazeAlg::Location& player) const;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 speed;				// Speed at which mother travels (MazePiece / sec)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* skeletal_mesh;

	
public:	
	// Sets default values for this actor's properties
	AMother();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Gets Mother's (x,y,z) location in space
	FVector getVectorLocation() const {
		return this->vect_location;
	}

	// Gets Mother's (r,c) location in the maze
	MazeAlg::Location getMazeLocation() const {
		return this->maze_location;
	}

	// Sets Mother's (r,c) location in the maze
	void setMazeLocation(const MazeAlg::Location& maze_location) {
		this->maze_location = maze_location;
	}

	// Sets Mother's (x,y,z) location in space
	void setVectorLocation(const FVector& vect_location) {
		this->vect_location = vect_location;
	}
    
    // Set's approach to an attack_type enum type
    /*
     attack_type setApproach(){
        return direction;
     }
     */
	// Updates dir to be queue of FVectors which can be used to take mother to player
	bool update_dir(const TArray<TArray<char>>& maze, const MazeAlg::Location& player);

	// Move mother according to shortest path to player (use member variable dir)
	// Returns false if dir is empty
	bool move(float DeltaTime);

};

