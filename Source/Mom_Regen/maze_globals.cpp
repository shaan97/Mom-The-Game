#include "Mom_Regen.h"

#include "maze_globals.h"

using namespace MazeAlg;

FVector MazeAlg::determineLocation(int i, int j) {
	return MazeAlg::START_VECTOR + FVector(MazeAlg::PIECE_SIDE_LENGTH * i, MazeAlg::PIECE_SIDE_LENGTH * j, MazeAlg::HEIGHT_ABOVE_GROUND);
}

//Takes FVector and determines its location (r,c) in maze
/* TODO : This code needs to be tested! */
Location MazeAlg::convert(const FVector& pLocation)
{
	FVector temp = pLocation - START_VECTOR;

	int r = (temp.Y / MazeAlg::MAZE_SIDE_LENGTH) + ((int)(temp.Y) % (int)(MazeAlg::MAZE_SIDE_LENGTH) >= MazeAlg::MAZE_SIDE_LENGTH / 2 ? 1 : 0);
	int c = (1.0 / MazeAlg::MAZE_SIDE_LENGTH)*(temp.X) + ((int)(temp.X) % (int)(MazeAlg::MAZE_SIDE_LENGTH) >= MazeAlg::MAZE_SIDE_LENGTH / 2 ? 1 : 0);

	return Location(-r, -c); // This might be incorrect
}