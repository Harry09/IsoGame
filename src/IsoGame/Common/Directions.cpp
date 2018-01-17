#include "Directions.h"

namespace Directions
{
	static std::map<Direction, char[3]> s_directionsName;

	const char *getDirectionName(Direction dir)
	{
		return s_directionsName[dir];
	}

	void init()
	{
		strcpy_s(s_directionsName[E], "E");
		strcpy_s(s_directionsName[N], "N");
		strcpy_s(s_directionsName[S], "S");
		strcpy_s(s_directionsName[W], "W");
		strcpy_s(s_directionsName[NE], "NE");
		strcpy_s(s_directionsName[NW], "NW");
		strcpy_s(s_directionsName[SE], "SE");
		strcpy_s(s_directionsName[SW], "SW");
	}
}