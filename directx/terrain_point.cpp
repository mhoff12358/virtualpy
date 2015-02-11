#include "terrain_point.h"

TerrainPoint linearInterpolateTerrainPoint(TerrainPoint& a, TerrainPoint& b, float x) {
	TerrainPoint return_point;
	return_point.height = (1-x)*a.height + x*b.height;
	for (int i = 0; i < 4; i++) {
		return_point.color[i] = (1-x)*a.color[i] + x*b.color[i];
	}
	return return_point;
}

TerrainPoint squareInterpolateTerrainPoint(TerrainPoint& aa, TerrainPoint& ba, TerrainPoint& ab, TerrainPoint& bb, float x, float y) {
	// Interpolates aa and ba along the x axis to get a on the y axis.
	// Interpolates ab and bb along the x axis to get b on the y axis.
	// Then interpolates a and b along the y axis.
	TerrainPoint a = linearInterpolateTerrainPoint(aa, ba, x);
	TerrainPoint b = linearInterpolateTerrainPoint(ab, bb, x);
	return linearInterpolateTerrainPoint(a, b, y);
}