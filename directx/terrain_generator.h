#include <array>

#include "terrain_point.h"

#ifndef __TERRAIN_GENERATOR_H_INCLUDED__
#define __TERRAIN_GENERATOR_H_INCLUDED__

class TerrainGenerator {
public:
	virtual ~TerrainGenerator() {};
	virtual TerrainPoint createPoint(std::array<int, 2> point_index);
};

#endif
