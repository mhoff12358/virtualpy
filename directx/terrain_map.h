#include <unordered_map>
#include <array>

#include "terrain_writer.h"
#include "terrain_point.h"
#include "terrain_generator.h"

#ifndef __TERRAIN_MAP_H_INCLUDED__
#define __TERRAIN_MAP_H_INCLUDED__

class TerrainMap {
public:
	TerrainMap() {};
	TerrainMap(TerrainGenerator* tg, TerrainWriter* tw) : generator(tg), writer(tw) {};

	TerrainPoint& getPoint(std::array<int, 2> point_index);
	TerrainPoint getInterpolatedPoint(std::array<float, 2> point_coords);

	void savePoints();
	void loadPoints();

	void setGenerator(TerrainGenerator* g);
	void setWriter(TerrainWriter* w);

private:
	TerrainGenerator* generator;
	TerrainWriter* writer;

	std::unordered_map<std::array<int, 2>, TerrainPoint> point_map;
};

#endif
