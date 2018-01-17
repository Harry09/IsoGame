#pragma once

#include <map>

namespace Directions
{
	enum Direction
	{
		N = 10,
		E = 01,
		S = 20,
		W = 02,
		NE = 11,
		NW = 12,
		SE = 21,
		SW = 22,
		NONE = 0
	};


	const char *getDirectionName(Direction dir);

	void init();
}