#include "Mom_Regen.h"

#include "maze_globals.h"

FVector MazeAlg::determineLocation(int i, int j) {
	return MazeAlg::START_VECTOR + FVector(MazeAlg::PIECE_SIDE_LENGTH * i, MazeAlg::PIECE_SIDE_LENGTH * j, MazeAlg::HEIGHT_ABOVE_GROUND);
}

//Takes FVector and determines its location (r,c) in maze
Location MazeAlg::convert(const FVector& pLocation)
{
	FVector temp = pLocation - this->vect_location;

	int r = (temp.Y / MazeAlg::MAZE_SIDE_LENGTH) + ((int)(temp.Y) % (int)(MazeAlg::MAZE_SIDE_LENGTH) >= MazeAlg::MAZE_SIDE_LENGTH / 2 ? 1 : 0);
	int c = (1.0 / MazeAlg::MAZE_SIDE_LENGTH)*(temp.X) + ((int)(temp.X) % (int)(MazeAlg::MAZE_SIDE_LENGTH) >= MazeAlg::MAZE_SIDE_LENGTH / 2 ? 1 : 0);

	return Location(maze_location.r - r, maze_location.c - c);
}