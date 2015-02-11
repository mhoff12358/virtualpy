
#include <array>
#include <cmath>
#include <climits>
#include <vector>

#include "terrain_generator.h"
#include "my_math.h"

#ifndef __SIMPLEX_GENERATOR_H_INCLUDED__
#define __SIMPLEX_GENERATOR_H_INCLUDED__

/*
 This class takes in world coordinates (the std::array<int, 2> point_index)
 and maps them to height values. It does that by summing the height values
 obtained from multiple layers of noise. For any given world coordinate, the
 layer will generate a height based on three factors, frequency, amplitude, 
 and seed. Frequency determines how world coordinates are mapped to noise
 coordinates, amplitude determines how the height of generated points is scaled
 for the final value, and seed affects the random generation that determines
 how points are generated.

 The number of layers, as well as their frequency, amplitude, and seed aren't
 fixed, and are a function of the point index.
*/

 struct LayerValues {
 	float amplitude;
 	float frequency;
 	int seed;
 };

class SimplexGenerator : public TerrainGenerator {
public:
	SimplexGenerator(std::vector<LayerValues> lay) : layers(lay) {};

	TerrainPoint createPoint(std::array<int, 2> point_index);

private:

	float generateLayerHeight(LayerValues& layer, std::array<int, 2> point_index);
	float generateLayerPoint(LayerValues& layer, std::array<int, 2> layer_coords);

	std::vector<LayerValues> layers;

	static float cosineLinearInterpolate(float a, float b, float x);
	static float cosineSquareInterpolate(float aa, float ab, float ba, float bb, float x, float y);
	static unsigned int intToUint(int a);
	static unsigned int mix(unsigned int a, unsigned int b, unsigned int c);
};

#endif
