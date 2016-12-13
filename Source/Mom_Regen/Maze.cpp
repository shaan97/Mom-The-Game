// Fill out your copyright notice in the Description page of Project Settings.

#include "Mom_Regen.h"
#include "Maze.h"
#include "Wall.h"
#include "Empty.h"
#include "Dash.h"
#include "Four_Way.h"
#include "L_Piece.h"
#include "T_Piece.h"
#include "Vert_Line.h"
#include "maze_globals.h"
#include <queue>
#include <unordered_set>

using namespace std;
using namespace MazeAlg;


Location getStart(TArray<TArray<char>> maze);
pair<Location, int> longestPath(TArray<TArray<char>> maze, Location startL);
bool interestingMaze(const TArray<TArray<char>>& maze, pair<Location, Location>& p);
std::queue<Location> returnPath(TArray<TArray<char>> maze, Location start, Location end);

// Evaluates entry in the char maze and returns the appropriate MazePiece value for that location
MazeAlg::maze_piece eval_piece(const TArray<TArray<char>>& maze, int r, int c, int &rot);

std::queue<Location> returnPath(TArray<TArray<char>> maze, Location start, Location end)
{
	std::queue<std::queue<Location> > path;
	path.push(std::queue<Location>());
	path.front().push(start);
	if (path.front().back() == end)
		return path.front();
	//Point has (x,y). We will interpret that as (r,c) for row and column, respectively
	while (!path.empty()) {

		int r = path.front().back().r;
		int c = path.front().back().c;
		if (c > 0 && c < maze[r].Num() && maze[r][c - 1] != 'X') {
			std::queue<Location> newDir = path.front(); //Copies path.front()'s data into a new std::queue
			newDir.push(Location(r, c - 1));
			if (newDir.back() == end)
				return newDir;

			path.push(newDir);					//pushes the new std::queue into path
			maze[r][c - 1] = 'X';
		}

		if (c >= 0 && c < maze[r].Num() - 1 && maze[r][c + 1] != 'X') {
			std::queue<Location> newDir = path.front();
			newDir.push(Location(r, c + 1));
			if (newDir.back() == end)
				return newDir;

			path.push(newDir);
			maze[r][c + 1] = 'X';
		}
		if (r >= 0 && r < maze.Num() - 1 && maze[r + 1][c] != 'X') {
			std::queue<Location> newDir = path.front();
			newDir.push(Location(r + 1, c));
			if (newDir.back() == end)
				return newDir;

			path.push(newDir);		//Copies data into a new std::queue and pops old std::queue
			maze[r + 1][c] = 'X';
		}
		if (r > 0 && r < maze.Num() && maze[r - 1][c] != 'X') {
			std::queue<Location> newDir = path.front();
			newDir.push(Location(r - 1, c));
			if (newDir.back() == end)
				return newDir;

			path.push(newDir);		//Copies data into a new std::queue and pops old std::queue
			maze[r - 1][c] = 'X';
		}
		path.pop();
	}
	return std::queue<Location>();
}

bool interestingMaze(const TArray<TArray<char>>& maze, pair<Location, Location>& p)
{
	Location start = getStart(maze);
	pair<Location, int> temp_path = longestPath(maze, start);
	Location end = temp_path.first;
	p.first = start;
	p.second = end;
	int pathSize = temp_path.second;

	//cout << start.r << " " << start.c << endl;
	//cout << end.r << " " << end.c << endl;

	//make a queue with all the points accessed in the path

	//check number of .s and Xs
	/*int numberOpen = 0;
	for (int i = 0; i < MAX_ROWS; i++)
	{
	for (int j = 0; j < MAX_COLS; j++)
	{
	if (maze[i][j] == '.')
	numberOpen++;
	}
	}
	if (numberOpen < ((MAX_ROWS * MAX_COLS) / 4))
	result1 = false;

	if (result1)
	pass1++;
	*/


	//evaluate all choices
	MazeAlg::maze_piece piece;
	int rotation = 0;
	int numChoices = 0;
	/*
	for (int i = 0; i < MAX_ROWS; i++)
	{
	for (int j = 0; j < MAX_COLS; j++)
	{
	piece = Maze::eval_piece(maze, i, j, rotation);
	if (piece == MazeAlg::T)
	numChoices = numChoices + 2;
	else if (piece == MazeAlg::FOUR_WAY)
	numChoices = numChoices + 3;
	}
	}

	if (numChoices < (MAX_ROWS * 5))
	result = false;*/

	int probability = 0;
	//evaluate number of choices by evaluating each Location in the path

	queue<Location> path = returnPath(maze, start, end);

	while (!path.empty())
	{
		Location pos = path.front();
		path.pop();
		piece = eval_piece(maze, pos.r, pos.c, rotation);
		if (piece == MazeAlg::T)
			probability = probability + 2;
		else if (piece == MazeAlg::FOUR_WAY)
			probability = probability + 3;
	}

	if (probability < 50)
		return false;
	

	//check if the longest path is atleast a certain number of points
	if (pathSize < ((maze.Num() * maze[0].Num()) / 50))
		return false;

	
	return true;
}

//returns the Location of the end and the number of steps to get to the end as a pair

pair<Location, int> longestPath(TArray<TArray<char>> maze, Location startL)
{
	pair<Location, int> start = make_pair(startL, 0);
	pair<Location, int> end = make_pair(Location(0, 0), 0);
	queue<pair<Location, int> > longest;
	longest.push(start);
	int MAX_ROWS = maze.Num();
	int MAX_COLS = maze[0].Num();
	while (!longest.empty())
	{
		end = longest.front();
		longest.pop();
		maze[end.first.r][end.first.c] = 'X';
		if (end.first.r > 0 && maze[end.first.r - 1][end.first.c] == '.') {
			longest.push(make_pair(Location(end.first.r - 1, end.first.c), end.second + 1));
		}
		if (end.first.c > 0 && maze[end.first.r][end.first.c - 1] == '.') {
			longest.push(make_pair(Location(end.first.r, end.first.c - 1), end.second + 1));
		}
		if (end.first.r < MAX_ROWS - 1 && maze[end.first.r + 1][end.first.c] == '.') {
			longest.push(make_pair(Location(end.first.r + 1, end.first.c), end.second + 1));
		}
		if (end.first.c < MAX_COLS - 1 && maze[end.first.r][end.first.c + 1] == '.') {
			longest.push(make_pair(Location(end.first.r, end.first.c + 1), end.second + 1));
		}
	}
	return end;
}

//getStart returns the starting position by randomly choosing a corner and finding the first '.'

Location getStart(TArray<TArray<char>> maze)
{
	int MAX_ROWS = maze.Num();
	int MAX_COLS = maze[0].Num();
	int corner = 0;
	int r, c;
	switch (corner) {
	case 0:
		for (r = 1; r < MAX_ROWS - 1; r++)
		{
			bool done = false;
			for (c = 1; c < MAX_COLS - 1; c++)
			{
				if (maze[r][c] == '.')
				{
					done = true;
					break;
				}
			}
			if (done)
				break;
		}
		break;
	case 1:
		for (r = 1; r < MAX_ROWS - 1; r++)
		{
			bool done = false;
			for (c = MAX_COLS - 2; c > 0; c--)
			{
				if (maze[r][c] == '.') {
					done = true;
					break;
				}
			}
			if (done)
				break;
		}
		break;
	case 2:
		for (r = MAX_ROWS - 2; r > 0; r--)
		{
			bool done = false;
			for (c = 1; c < MAX_COLS - 1; c++)
			{
				if (maze[r][c] == '.')
				{
					done = true;
					break;
				}
			}
			if (done)
				break;
		}
		break;
	case 3:
		for (r = MAX_ROWS - 2; r > 0; r--)
		{
			bool done = false;
			for (c = MAX_COLS - 2; c > 0; c--)
			{
				if (maze[r][c] == '.') {
					done = true;
					break;
				}
			}
			if (done)
				break;
		}
		break;
	}
	return Location(r, c);
}
bool bad_eval(const TArray<TArray<char>>& maze) {
	return false;
}

// Sets default values
AMaze::AMaze()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	// Initializes obj_maze to all nullptr
	TArray<AMazePiece*> rows;
	rows.Init(nullptr, MazeAlg::MAZE_SIDE_LENGTH);
	obj_maze.Init(rows, MazeAlg::MAZE_SIDE_LENGTH);
	
	// Initializes maze to all walls
	TArray<char> row;
	row.Init('X', MazeAlg::MAZE_SIDE_LENGTH);
	maze.Init(row, MazeAlg::MAZE_SIDE_LENGTH);

	// Randomly regenerates the maze
	regenerate(interestingMaze);
	
}

// Called when the game starts or when spawned
void AMaze::BeginPlay()
{
	Super::BeginPlay();
	respawn();
}

// Called every frame
void AMaze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool inBounds(const int size, const int x) {
	return x > 0 && x < size - 1;
}

bool inBounds(const int size, const int r, const int c, const char dir, const int magnitude) {
	enum Direction : char { NORTH = 1, SOUTH = 2, EAST = 3, WEST = 4 };
	Direction d = (Direction)(dir);
	switch (d) {
	case NORTH:
		return inBounds(size, r - (magnitude - 1)) && inBounds(size, c);
	case SOUTH:
		return inBounds(size, r + (magnitude - 1)) && inBounds(size, c);
	case EAST:
		return inBounds(size, r) && inBounds(size, c + (magnitude - 1));
	case WEST:
		return inBounds(size, r) && inBounds(size, c - (magnitude - 1));
	default:
		return false;
	}
}



bool canPlaceSpace(const TArray<TArray<char>>& maze, const int r, const int c) {
	if (c < 1 || r < 1 || r >= maze.Num() - 1 || c >= maze[0].Num() - 1)
		return false;

	int count = 0;
	if (r > 1 && maze[r - 1][c] == '.')
		count++;
	if (r + 1 < maze.Num() - 1 && maze[r + 1][c] == '.')
		count++;
	if (c > 1 && maze[r][c - 1] == '.')
		count++;
	if (c + 1 < maze[0].Num() - 1 && maze[r][c + 1] == '.')
		count++;

	return count <= 1;
}

void fillWith(TArray<TArray<char>>& maze, char val) {
	for (int i = 0; i < maze.Num(); i++) {
		for (int j = 0; j < maze[i].Num(); j++) {
			maze[i][j] = val;
		}
	}
}

// Randomly regenerates maze
bool AMaze::regenerate(bool testMaze(const TArray<TArray<char>>&, pair<Location, Location>&)) {
	//Fill maze with X, so we can begin randomization
	fillWith(maze, 'X');

	int r_start = maze.Num() / 2, c_start = maze[0].Num() / 2;
	bool badMaze = true; //Using this in par with testMaze to evaluate the maze. Right now, it is obviously not a good maze
	const int TOTAL_TRIES = 50;
	int numTries = 0; //After TOTAL_TRIES tries, something is wrong, so let's return false
	pair<Location, Location> loc;
	do {
		queue<Location> path;
		path.push(Location(r_start, c_start));
		maze[r_start][c_start] = '.';

		while (!path.empty()) {
			/* We will be picking a random number of directions, d, where 2 <= d <= 4; and out of that number we will randomly
			select random directions North, South, East, West. For each of those we will choose a random magnitude, m, from
			1 <= m <= maze.size() / 4.
			*/
			int row = path.front().r;
			int col = path.front().c;
			if (inBounds(maze.Num(), row) && inBounds(maze[0].Num(), col))
				maze[row][col] = '.';

			int d = (rand() % 3) + 2;	//number of directions

			unordered_set<int> indicesUsed;
			indicesUsed.reserve(d);

			for (int i = 0; i < d; i++) {
				//pick random index. If it is false, make it true and set magnitude. Else try again (so decrement i)
				int dir_selection = rand() % 4;

				if (indicesUsed.find(dir_selection) != indicesUsed.end() && indicesUsed.size() != d) {
					//index already used. Try again
					--i;
					continue;
				}

				//Successful choice of index
				indicesUsed.insert(dir_selection);



				//Potential early termination: No room to move forward, and nothing ends up being pushed onto the queue
				int mag = (rand() % (maze.Num() / 4)) + 1;
				switch (dir_selection) {
				case 0: {
					//North
					int j = 1;
					bool fail = false;
					for (j = 1; j <= mag; j++) {
						if (!canPlaceSpace(maze, row - j, col)) {
							fail = true;
							break;
						}
						maze[row - j][col] = '.';
					}

					if (!fail)
						path.push(Location(row - j, col));

					break;
				}
				case 1: {
					//South
					int j = 1;
					bool fail = false;
					for (j = 1; j <= mag; j++) {
						if (!canPlaceSpace(maze, row + j, col)) {
							fail = true;
							break;
						}
						maze[row + j][col] = '.';
						//cerr << "(" << path.front().r + j << "," << path.front().c << ")" << endl;
					}
					if (!fail)
						path.push(Location(row + mag, col));

					break;
				}
				case 2: {
					//East
					int j = 1;
					bool fail = false;
					for (j = 1; j <= mag; j++) {
						if (!canPlaceSpace(maze, row, col + j)) {
							fail = true;
							break;
						}
						maze[row][col + j] = '.';
						//cerr << "(" << path.front().r << "," << path.front().c + j << ")" << endl;
					}
					if (!fail)
						path.push(Location(row, col + mag));

					break;
				}
				case 3: {
					//West
					int j = 1;
					bool fail = false;
					for (j = 1; j <= mag; j++) {
						if (!canPlaceSpace(maze, row, col - j)) {
							fail = true;
							break;
						}
						maze[row][col - j] = '.';
						//cerr << "(" << path.front().r << "," << path.front().c - j << ")" << endl;
					}
					if (!fail)
						path.push(Location(row, col - mag));

					break;
				}

				}

			}
			path.pop();


		}

		//Now we use the function passed to evaluate whether the maze is valid. If not, we should keep randomizing.
		badMaze = !testMaze(maze, loc);
		while (badMaze) {
			r_start = rand() % maze.Num();
			c_start = rand() % maze[0].Num();
			if (maze[r_start][c_start] == '.' && (canPlaceSpace(maze, r_start + 1, c_start) || canPlaceSpace(maze, r_start - 1, c_start) || canPlaceSpace(maze, r_start, c_start + 1) || canPlaceSpace(maze, r_start, c_start - 1)))
				break;
		}
		numTries++;

		if (badMaze && numTries >= TOTAL_TRIES) {
			this->start = loc.first;
			this->end = loc.second;
			return false;
		}

	} while (badMaze);
	
	this->start = loc.first;
	this->end = loc.second;
	return true;
}

// TODO : Respawns maze in the game
bool AMaze::respawn() {
	UWorld* World = GetWorld();
	bool failure_detected = true;
	if (World) {
		//World exists
		failure_detected = false;
		FActorSpawnParameters param;	//Sets up some parameter values for spawning
		param.Owner = this;
		param.Instigator = Instigator;
		
		int numRows = maze.Num(), numCols;	//Remember numRows and numCols for minor optimization purposes
		
		for (int i = 0; i < numRows; i++) {
			TArray<AMazePiece*>& arr = obj_maze[i];	//Figure out what row being examined beforehand for minor optimization
			numCols = maze[i].Num();
			for (int j = 0; j < numCols; j++) {
				int rot;
				maze_piece piece = eval_piece(maze, i, j, rot);	//Figure out what piece should be spawned at this (r,c)
				
				//Spawn the appropriate Maze Piece
				switch (piece) {
				case T:
					arr[j] = World->SpawnActor<AT_Piece>(AT_Piece::StaticClass(), determineLocation(i, j), FRotator(0, rot, 0), param);
					break;
				case L:
					arr[j] = World->SpawnActor<AL_Piece>(AL_Piece::StaticClass(), determineLocation(i, j), FRotator(0, rot, 0), param);
					break;
				case VERT_LINE:
					arr[j] = World->SpawnActor<AVert_Line>(AVert_Line::StaticClass(), determineLocation(i, j), FRotator(0, rot, 0), param);
					break;
				case DASH:
					arr[j] = World->SpawnActor<ADash>(ADash::StaticClass(), determineLocation(i, j), FRotator(0, rot, 0), param);
					break;
				case EMPTY:
				case WALL:
					arr[j] = nullptr;
					break;
				case FOUR_WAY:
					arr[j] = World->SpawnActor<AFour_Way>(AFour_Way::StaticClass(), determineLocation(i, j), FRotator(0, rot, 0), param);
					break;
				default:
					failure_detected = true;
					break;
				}

			}
		}
	}
	return !failure_detected;
}

// Evaluates entry in the char maze and returns the appropriate maze_piece value for that location
MazeAlg::maze_piece eval_piece(const TArray<TArray<char>>& maze, int r, int c, int &rot) {
	if ((r == 0 && r == c) || maze[r][c] == 'X') {
		rot = 0;
		return WALL;
	}

	short count = 0;
	char info = 0; //last four bits are defined as down, left, up, right
	if (c > 0 && maze[r][c - 1] == '.') {
		info += 4;
		count++;
	}
	if (c < maze[r].Num() - 1 && maze[r][c + 1] == '.') {
		info += 1;
		count++;
	}
	if (r > 0 && maze[r - 1][c] == '.') {
		info += 2;
		count++;
	}
	if (r < maze.Num() - 1 && maze[r + 1][c] == '.') {
		info += 8;
		count++;
	}

	switch (count) {
	case 0:
		rot = 0;
		return EMPTY;
	case 1:
		rot = log2(info) * 90;
		return DASH;
	case 2:
		if (info % 3 == 0) {
			if (info <= 6)
				rot = (((info / 3) - 1) * 90);
			else {
				rot = (info == 9 ? 270 : 180);
			}
			return L;
		}
		else {
			rot = (info == 5 ? 180 : 0);
			return VERT_LINE;
		}
	case 3:
		// TODO: Check to see if this still works, since I had to cast to unsigned char instead of char.
		rot = log2(~(info + (unsigned char)(240))) * 90;		//DEFAULT T IS ACTUALLY ROTATED ON IT'S SIDE. IT IS AN H WITHOUT THE LEFT BAR.
		return T;
	case 4:
		rot = 0;
		return FOUR_WAY;
	default:
		return WALL;
	}


}

