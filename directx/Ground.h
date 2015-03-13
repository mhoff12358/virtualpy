#ifndef __GROUND_H_INCLUDED__
#define __GROUND_H_INCLUDED__

#include "RenderMode.h"
#include "Vertices.h"
#include "ViewState.h"
#include "terrain_map.h"
#include "simplex_generator.h"
#include "GridGenerator.h"
#include "Logging.h"

class Ground {
public:
	Ground(ViewState* vs) : view_state(vs), terrain_generator(std::vector < LayerValues > {{
			{2.0f, 1.0f / 8.0f, 0}}}),
		terrain_map(&terrain_generator, &terrain_writer) {};

	void SetRenderArea(std::array<int, 2> area_min, std::array<int, 2> area_max);

	void Draw(const RenderMode& render_mode);

private:
	void FillVertexData(std::array<int, 2> point_index);

	ViewState* view_state;

	SimplexGenerator terrain_generator;
	TerrainWriter terrain_writer;
	TerrainMap terrain_map;

	//ModelGenerator terrain_model_gen;
	Model terrain_model;
};

#endif