#include "terrain_generator.h"

TerrainPoint TerrainGenerator::createPoint(std::array<int, 2> point_index) {
	TerrainPoint point;
	point.height = point_index[0];
	return point;
}
