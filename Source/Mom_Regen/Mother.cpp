// Fill out your copyright notice in the Description page of Project Settings.

#include "Mom_Regen.h"
#include "Mother.h"

#include <queue>
#include "maze_globals.h"

using namespace std;
using namespace MazeAlg;

// Sets default values
AMother::AMother() : speed(1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UStaticMeshComponent* static_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> applyMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (applyMesh.Object) {
		static_mesh->SetStaticMesh(applyMesh.Object);
	}
}

// Called when the game starts or when spawned
void AMother::BeginPlay()
{
	Super::BeginPlay();
}

bool AMother::move(float DeltaTime) {
	if (dir.empty())	// Cannot move, so return false
		return false;

	if (dir.front().Size() <= 0) {
		// FVector has no magnitude, so we cannot use this direction
		dir.pop();
		this->maze_location = convert(this->vect_location);		// Completed an FVector, so update maze (r,c) location
		if (dir.empty())	// Cannot move, so return false
			return false;
	}

	// There is a nonzero FVector available to use to move
	const int CENT_IN_METER = 100;	// Number of centimeters in a meter
	FVector delta_r = dir.front() * .5;	// FVector for change in distance
	this->vect_location += delta_r;											// Update vector location to new location
	this->SetActorLocation(this->vect_location);							// Set actor location to new location
	dir.front() -= delta_r;													// Used delta_r of dir.front(), so remove it for next tick

	return true;


}

// Called every frame
void AMother::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	/* NOTE : The AGame class invokes update_dir(...), not AMother */
	// Move Mother
	move(DeltaTime);

}


// Updates dir to be queue of FVectors which can be used to take mother to player. Returns false if it fails
bool AMother::update_dir(const TArray<TArray<char>>& maze, const Location& player) {
	std::queue<Location> p = shortestPath(maze, player);
	if (p.empty())
		return false;

	Location temp = p.front();
	p.pop();
	if (p.empty())
		return true;	//If there is only one Point, then the Mother has already found the Player

	

	queue<FVector> v;
	while (!p.empty()) {
		Location temp2 = p.front();
		temp = temp2 - temp;
		if (temp.r == 1)
			v.push(FVector(PIECE_SIDE_LENGTH, 0, 0));
		else if (temp.r == -1)
			v.push(FVector(-PIECE_SIDE_LENGTH, 0, 0));
		else if (temp.c == 1)
			v.push(FVector(0, PIECE_SIDE_LENGTH, 0));
		else if (temp.c == -1)
			v.push(FVector(0, -PIECE_SIDE_LENGTH, 0));
		else
			v.push(FVector(0, 0, 0));

		temp = temp2;
		p.pop();
	}

	dir = v;
	return true;
	
}


//Returns queue of Locations for shortest path from Mother to Player
std::queue<Location> AMother::shortestPath(TArray<TArray<char>> maze, const Location& player) const {
	queue<queue<Location>> path;
	path.push(queue<Location>());

	path.front().push(maze_location);

	if (path.front().back().r >= maze.Num() || path.front().back().r < 0 || path.front().back().c < 0 || path.front().back().c >= maze[0].Num())
		return path.front();

	if (path.front().back() == player)
		return path.front();

	int rows = maze.Num();
	int cols = maze[0].Num();
	//Location has (r,c) for row and column, respectively
	while (!path.empty()) {

		int r = path.front().back().r;
		int c = path.front().back().c;



		if (c > 0 && c < cols && maze[r][c - 1] != 'X') {
			queue<Location> newDir = path.front(); //Copies path.front()'s data into a new queue
			newDir.push(Location(r, c - 1));
			if (newDir.back() == player)
			{
				return newDir;
			}

			path.push(newDir);					//pushes the new queue into path
			maze[r][c] = 'X';
		}

		if (c >= 0 && c < cols - 1 && maze[r][c + 1] != 'X') {
			queue<Location> newDir = path.front();
			newDir.push(Location(r, c + 1));
			if (newDir.back() == player)
			{
				return newDir;
			}

			path.push(newDir);
			maze[r][c] = 'X';
		}
		if (r >= 0 && r < rows - 1 && maze[r + 1][c] != 'X') {
			queue<Location> newDir = path.front();
			newDir.push(Location(r + 1, c));
			if (newDir.back() == player)
			{
				return newDir;
			}

			path.push(newDir);		//Copies data into a new queue and pops old queue
			maze[r][c] = 'X';
		}
		if (r > 0 && r < rows && maze[r - 1][c] != 'X') {
			queue<Location> newDir = path.front();
			newDir.push(Location(r - 1, c));
			if (newDir.back() == player)
			{
				return newDir;
			}

			path.push(newDir);		//Copies data into a new queue and pops old queue
			maze[r][c] = 'X';
		}
		path.pop();
	}

	return queue<Location>();
}