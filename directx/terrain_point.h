
#include <array>
#include <cmath>
#include <climits>

#ifndef __TERRAIN_POINT_H_INCLUDED__
#define __TERRAIN_POINT_H_INCLUDED__

namespace std
{
	template<>
	struct hash<array<int, 2>>
	{
		typedef array<int, 2> argument_type;
		typedef std::size_t result_type;

		result_type operator()(argument_type const& s) const
		{
			return std::hash<long int>()((s[1] << sizeof(int)) | s[0]);
		}
	};
}

class TerrainPoint {
public:
	float height;
	std::array<float, 4> color;

	TerrainPoint(float sheight, std::array<float, 4> scolor) : height(sheight), color(scolor) {};
	TerrainPoint() : height(0.0f), color({{0.0f, 0.0f, 0.0f, 0.0f}}) {};
};


TerrainPoint linearInterpolateTerrainPoint(TerrainPoint& a, TerrainPoint& b, float x);

TerrainPoint squareInterpolateTerrainPoint(TerrainPoint& aa, TerrainPoint& ba, TerrainPoint& ab, TerrainPoint& bb, float x, float y);

#endif