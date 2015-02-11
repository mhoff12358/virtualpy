#include "terrain_map.h"

/*
Tries to pull in the terrain data for the given index from memory if possible,
falling back to requesting a new one if necessary. 
*/
TerrainPoint& TerrainMap::getPoint(std::array<int, 2> point_index) {
	auto found_point = point_map.find(point_index);
	if (found_point == point_map.end()) {
		found_point = point_map.emplace(point_index, generator->createPoint(point_index)).first;
	}
	return found_point->second;
}

TerrainPoint TerrainMap::getInterpolatedPoint(std::array<float, 2> point_coords) {
	// point_coords is where we want to generate a height for. Heights will
	// need to be generated for the surrounding integer coordinate points and
	// then interpolated to this point.
	std::array<int, 2> min_coords = {{static_cast<int>(lround(floor(point_coords[0]))),
		                              static_cast<int>(lround(floor(point_coords[1])))}};
	std::array<int, 2> max_coords = {{static_cast<int>(lround(ceil(point_coords[0]))),
		                              static_cast<int>(lround(ceil(point_coords[1])))}};
	return squareInterpolateTerrainPoint(
		getPoint({{min_coords[0], min_coords[1]}}),
		getPoint({{max_coords[0], min_coords[1]}}),
		getPoint({{min_coords[0], max_coords[1]}}),
		getPoint({{max_coords[0], max_coords[1]}}),
		point_coords[0]-min_coords[0],
		point_coords[1]-min_coords[1]);
}

void TerrainMap::savePoints() {
	writer->writeAllPointsToFile(point_map);
}

void TerrainMap::loadPoints() {
	writer->readAllPointsFromFile(point_map);
}

void TerrainMap::setGenerator(TerrainGenerator* g) {
	generator = g;
}

void TerrainMap::setWriter(TerrainWriter* w) {
	writer = w;
}