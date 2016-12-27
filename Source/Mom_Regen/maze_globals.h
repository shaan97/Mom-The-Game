#pragma once

#ifndef MAZE_GLOBALS_H
#define MAZE_GLOBALS_H

#include "Mom_Regen.h"


namespace MazeAlg {
	enum maze_piece { EMPTY, T, L, VERT_LINE, DASH, WALL, FOUR_WAY };
	const int MAZE_SIDE_LENGTH = 50;
	const int PIECE_SIDE_LENGTH = 190;
	const int HEIGHT_ABOVE_GROUND = 0;
	const FVector START_VECTOR(-260, -60, 100);
	
	FVector determineLocation(int i, int j);

	

	struct Location {
		int r, c;
		Location(int x = 0, int y = 0) : r(x), c(y)
		{}

		bool operator==(const Location& l) {
			return this->r == l.r && this->c == l.c;
		}

		bool operator!=(const Location& l) {
			return !(*this == l);
		}

		Location operator-(const Location& l) {
			return Location(this->r - l.r, this->c - l.c);
		}

	};

	//Takes FVector and determines its location (r,c) in maze
	Location convert(const FVector& pLocation);
}





#endif