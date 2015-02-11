
#ifndef __GRID_GENERATOR_H_INCLUDED__
#define __GRID_GENREATOR_H_INCLUDED__

#include "terrain_generator.h"
#include "my_math.h"

class GridGenerator :
	public TerrainGenerator
{
public:
	TerrainPoint createPoint(std::array<int, 2> point_index);
};

#endif