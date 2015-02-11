#include "simplex_generator.h"

TerrainPoint SimplexGenerator::createPoint(std::array<int, 2> point_index) {
	TerrainPoint point;

	point.height = 0;
	// for (int i = 0; i < 3; i++) {
	// 	LayerValues layer = {(float)(10.0*pow(2, i)), (float)pow(0.5, 5*i), i};
	// 	point.height += generateLayerHeight(layer, point_index);
	// }
	/*LayerValues layer = {500.0f, 1.0f/64.0f, 0};
	point.height += generateLayerHeight(layer, point_index);
	layer = {250.0f, 1.0f/32.0f, 1};
	point.height += generateLayerHeight(layer, point_index);
	layer = {125.0f, 1.0f/16.0f, 2};
	point.height += generateLayerHeight(layer, point_index);*/
	for (LayerValues& layer : layers) {
		point.height += generateLayerHeight(layer, point_index);
	}

	point.color[0] = 172.0/255.0+17.0/255.0*randf(-1.0, 1.0);
	point.color[1] = 172.0/255.0+10.0/255.0*randf(-1.0, 1.0);
	point.color[2] = 93.0/255.0+5.0/255.0*randf(-1.0, 1.0);
	point.color[3] = 1.0f;

	return point;
}

float SimplexGenerator::generateLayerHeight(LayerValues& layer, std::array<int, 2> point_index) {
	// The world coordinates need to be multiplied by the frequency to convert
	// to layer coordinates.
	std::array<float, 2> layer_coords = {{point_index[0]*layer.frequency, point_index[1]*layer.frequency}};
	// layer_coords is where we want to generate a height for. Heights
	// will need to be generated for the surrounding integer coordinate points
	// and then interpolated to this point.
	std::array<int, 2> min_coords = {{static_cast<int>(lround(floor(layer_coords[0]))),
		                              static_cast<int>(lround(floor(layer_coords[1])))}};
	std::array<int, 2> max_coords = {{static_cast<int>(lround(ceil(layer_coords[0]))),
		                              static_cast<int>(lround(ceil(layer_coords[1])))}};
	return cosineSquareInterpolate(
		generateLayerPoint(layer, {{min_coords[0], min_coords[1]}}),
		generateLayerPoint(layer, {{max_coords[0], min_coords[1]}}),
		generateLayerPoint(layer, {{min_coords[0], max_coords[1]}}),
		generateLayerPoint(layer, {{max_coords[0], max_coords[1]}}),
		layer_coords[0]-min_coords[0],
		layer_coords[1]-min_coords[1]);
}

float SimplexGenerator::generateLayerPoint(LayerValues& layer, std::array<int, 2> layer_coords) {
	// Generates a random value between 0 and layer.amplitude, determined by
	// seed and the layer_coords.
	unsigned int random_value = mix(layer_coords[0], layer_coords[1], layer.seed);
	return ((double)random_value/UINT_MAX)*layer.amplitude;
}

float SimplexGenerator::cosineLinearInterpolate(float a, float b, float x) {
	// Interpolates between a and b, with a 0 x meaning a and a 1 x  meaning b.
	float proportion = (1-cos(x*3.1415927))*0.5;
	return (1-proportion)*a + proportion*b;
}

float SimplexGenerator::cosineSquareInterpolate(float aa, float ba, float ab, float bb, float x, float y) {
	// Interpolates aa and ba along the x axis to get a on the y axis.
	// Interpolates ab and bb along the x axis to get b on the y axis.
	// Then interpolates a and b along the y axis.
	float a = cosineLinearInterpolate(aa, ba, x);
	float b = cosineLinearInterpolate(ab, bb, x);
	return cosineLinearInterpolate(a, b, y);
}

unsigned int SimplexGenerator::intToUint(int a) {
	return *((unsigned int*)&a);
}

unsigned int SimplexGenerator::mix(unsigned int a, unsigned int b, unsigned int c) {
	// 96 bit hash, see https://gist.github.com/badboy/6267743,
	// Robert Jenkins' 96 bit Mix Function
	a=a-b;  a=a-c;  a=a^(c >> 13);
	b=b-c;  b=b-a;  b=b^(a << 8);
	c=c-a;  c=c-b;  c=c^(b >> 13);
	a=a-b;  a=a-c;  a=a^(c >> 12);
	b=b-c;  b=b-a;  b=b^(a << 16);
	c=c-a;  c=c-b;  c=c^(b >> 5);
	a=a-b;  a=a-c;  a=a^(c >> 3);
	b=b-c;  b=b-a;  b=b^(a << 10);
	c=c-a;  c=c-b;  c=c^(b >> 15);
	return c;
}
