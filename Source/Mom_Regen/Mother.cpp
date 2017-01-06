// Fill out your copyright notice in the Description page of Project Settings.

#include "Mom_Regen.h"
#include "Mother.h"

#include <queue>
#include "maze_globals.h"

using namespace std;
using namespace MazeAlg;

// Sets default values
AMother::AMother() : temp_speed(500), speed(0), isAttacking(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	skeletal_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mother Mesh Component"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> mesh_container(TEXT("SkeletalMesh'/Game/Mom/MomAnimationIdle1.MomAnimationIdle1'"));
	if (mesh_container.Succeeded()) {
		skeletal_mesh->SetSkeletalMesh(mesh_container.Object);
	}
	FVector scale(.1);
	this->SetActorScale3D(scale);
	this->skeletal_mesh->SetRelativeScale3D(scale);
	this->skeletal_mesh->SetCollisionProfileName(TEXT("OverlapAll"));

	const ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimObj(TEXT("AnimBlueprint'/Game/Mom/Mom_AnimBP.Mom_AnimBP'"));
	if(AnimObj.Succeeded())
		this->skeletal_mesh->SetAnimInstanceClass(AnimObj.Object->GetAnimBlueprintGeneratedClass());

	/*
	UStaticMeshComponent* static_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> applyMesh(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (applyMesh.Object) {
		static_mesh->SetStaticMesh(applyMesh.Object);
	}
	*/
}

// Called when the game starts or when spawned
void AMother::BeginPlay()
{
	Super::BeginPlay();
}

bool AMother::move() {
	if (dir.empty())	// Cannot move, so return false
		return false;
	
	FVector& vec = dir.front();
	if (vec.X != 0) {
		if (vec.X > 0) {
			// Moving right
			this->SetActorRotation(FRotator(0, -90, 0));
		}
		else
			this->SetActorRotation(FRotator(0, 90, 0));
	}
	else {
		if (vec.Y > 0) {
			// Moving down
			this->SetActorRotation(FRotator(0, 0, 0));
		}
		else
			this->SetActorRotation(FRotator(0, -180, 0));
	}
	

	this->vect_location += vec;					// Change Mother's vect_location to new (x,y,z)
	this->SetActorLocation(this->vect_location);		// Use vect_location to set actual in-game location
	this->maze_location = convert(this->vect_location);	// Update Mother's maze_location to new (r,c)
	dir.pop();											// Dequeue latest movement instruction
	
	return true;


}

// Called every frame
void AMother::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	/* NOTE : The AGame class invokes update_dir(...), not AMother */
	if (!dir.empty()) {
		speed = temp_speed;
		move();	// Move Mother
	}
	else {
		speed = 0;
	}

}


// Updates dir to be queue of FVectors which can be used to take mother to player. Returns false if it fails
bool AMother::update_dir(const TArray<TArray<char>>& maze, const Location& player) {
	// Make sure to move one last unit so alignment of Mother in maze falls in the center.
	while (!dir.empty() && dir.front().Z != END_MARKER)
		move();
	//dir.empty() || dir.front() == END_MARKER
	if (!dir.empty()) {
		move();
	}

	// Now update dir

	std::queue<Location> p = shortestPath(maze, player);
	if (p.empty())
		return false;

	Location temp = p.front();
	p.pop();
	if (p.empty())
		return true;	//If there is only one Point, then the Mother has already found the Player

	const int FPS = 30;
	double frac_block = ((double)(temp_speed) / FPS);// *PIECE_SIDE_LENGTH;
	double sum;
	queue<FVector> v;
	while (!p.empty()) {
		Location temp2 = p.front();
		temp = temp2 - temp;
		sum = 0;
		if (temp.r == 1) {
			while (sum + frac_block < PIECE_SIDE_LENGTH) {
				v.push(FVector(frac_block, 0, 0));
				sum += frac_block;
			}
			// sum + frac_block >= PIECE_SIDE_LENGTH

			// This catches the boundary case where adding another frac_block
			// will go over the maze piece size
			v.push(FVector(PIECE_SIDE_LENGTH - sum, 0, END_MARKER));
		}
		else if (temp.r == -1) {
			while (sum + frac_block < PIECE_SIDE_LENGTH) {
				v.push(FVector(-frac_block, 0, 0));
				sum += frac_block;
			}
			// sum + frac_block >= PIECE_SIDE_LENGTH

			// This catches the boundary case where adding another frac_block
			// will go over the maze piece size
			v.push(FVector(sum - PIECE_SIDE_LENGTH, 0, 0));
		}
		else if (temp.c == 1) {
			while (sum + frac_block < PIECE_SIDE_LENGTH) {
				v.push(FVector(0, frac_block, 0));
				sum += frac_block;
			}
			// sum + frac_block >= PIECE_SIDE_LENGTH

			// This catches the boundary case where adding another frac_block
			// will go over the maze piece size
			v.push(FVector(0, PIECE_SIDE_LENGTH - sum, 0));
		}
		else if (temp.c == -1) {
			while (sum + frac_block < PIECE_SIDE_LENGTH) {
				v.push(FVector(0, -frac_block, 0));
				sum += frac_block;
			}
			// sum + frac_block >= PIECE_SIDE_LENGTH

			// This catches the boundary case where adding another frac_block
			// will go over the maze piece size
			v.push(FVector(0, sum - PIECE_SIDE_LENGTH, 0));
		}
		else {
			v.push(FVector(0, 0, 0));
		}

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
	maze[path.front().back().r][path.front().back().c] = 'X';
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
			maze[r][c - 1] = 'X';
		}

		if (c >= 0 && c < cols - 1 && maze[r][c + 1] != 'X') {
			queue<Location> newDir = path.front();
			newDir.push(Location(r, c + 1));
			if (newDir.back() == player)
			{
				return newDir;
			}

			path.push(newDir);
			maze[r][c + 1] = 'X';
		}
		if (r >= 0 && r < rows - 1 && maze[r + 1][c] != 'X') {
			queue<Location> newDir = path.front();
			newDir.push(Location(r + 1, c));
			if (newDir.back() == player)
			{
				return newDir;
			}

			path.push(newDir);		//Copies data into a new queue and pops old queue
			maze[r + 1][c] = 'X';
		}
		if (r > 0 && r < rows && maze[r - 1][c] != 'X') {
			queue<Location> newDir = path.front();
			newDir.push(Location(r - 1, c));
			if (newDir.back() == player)
			{
				return newDir;
			}

			path.push(newDir);		//Copies data into a new queue and pops old queue
			maze[r - 1][c] = 'X';
		}
		path.pop();
	}

	return queue<Location>();
}