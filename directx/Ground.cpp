#include "Ground.h"

void Ground::SetRenderArea(std::array<int, 2> area_min, std::array<int, 2> area_max) {
	// Should only be called once, as currently the Model doesn't like being reinitialized.

	// Creates a single degenerate triangle strip representing the whole area.
	// Each sub-strip crosses along the x-axis, so the substrip has
	// area_max[0]-area_min[0] points and there are area_max[1]-area_min[1] strips.
	int area_width = area_max[0] - area_min[0];
	int area_height = area_max[1] - area_min[1];

	for (int strip_number = 0; strip_number < area_height; strip_number++) {
		FillVertexData(std::array<int, 2>{{area_min[0], strip_number + area_min[1]}});
		for (int column_number = 0; column_number <= area_width; column_number++) {
			FillVertexData(std::array<int, 2>{{column_number + area_min[0], strip_number + area_min[1]}});
			FillVertexData(std::array<int, 2>{{column_number + area_min[0], strip_number + area_min[1] + 1}});
		}
		FillVertexData(std::array<int, 2>{{area_width + area_min[0], strip_number + area_min[1] + 1}});
	}

	terrain_model = terrain_model_gen.DumpModel(view_state->device_interface, view_state->device_context);
}

void Ground::FillVertexData(std::array<int,2> point_index) {
	//OutputDebugString(("Ground Point: " + intToString(point_index[0]) + " " + intToString(point_index[1]) + "\n").c_str());

	TerrainPoint& terrain_point = terrain_map.getPoint(point_index);
	COLORVERTEX new_vertex = { (float)point_index[0], terrain_point.height, (float)point_index[1],
		terrain_point.color[0], terrain_point.color[1], terrain_point.color[2], terrain_point.color[3] };
	terrain_model_gen.AddVertex(&new_vertex);
}

void Ground::Draw(const RenderMode& render_mode) {
	// All view and projection matrix setup is assumed to be done by the caller
	render_mode.RenderModel(terrain_model);
}
