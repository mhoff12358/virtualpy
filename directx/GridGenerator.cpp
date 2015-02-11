#include "GridGenerator.h"

TerrainPoint GridGenerator::createPoint(std::array<int, 2> point_index) {
	TerrainPoint point;
	point.height = 0.0f;

	point.color[0] = eucmod(point_index[0], 2);
	//point.color[1] = eucmod(point_index[1], 2);
	point.color[1] = 0.0f;
	if (point_index[0] == 0 && point_index[1] == 0) {
		point.color[2] = 1.0f;
	}
	else {
		point.color[2] = 0.0f;
	}
	point.color[3] = 1.0f;

	return point;
}
