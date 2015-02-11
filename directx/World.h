#ifndef __WORLD_H_INCLUDED__
#define __WORLD_H_INCLUDED__

#include <array>

#include "PerspectiveCamera.h"
#include "InputHandler.h"
#include "ViewState.h"
#include "Model.h"
#include "ConstantBuffer.h"
#include "RenderMode.h"
#include "Texture.h"
#include "Vertices.h"
#include "Ground.h"
#include "ModeledEntity.h"
#include "DrawHandler.h"
#include "EntityFactory.h"

class World {
public:
	World(ViewState* vs);

	void Initialize(Camera* cam, InputHandler* ih);

	void UpdateLogic(const InputHandler& input_handler, int time_delta);

	void Draw(RenderMode& render_mode);

	std::array<float, 3> GetPlayerLocation();
	std::array<float, 4> GetPlayerOrientation();

private:
	ViewState* view_state;
	InputHandler* input_handler;
	Camera* player_camera;
	ConstantBuffer camera_transformation;

	std::array<float, 3> player_location;
	std::array<float, 4> player_orientation;

	// Temp stuff to be rendered, should be phased out.
	ModelGenerator triangle_gen;
	Model triangle;
	Texture texture;
	int shader_number;
	std::vector<std::pair<ConstantBuffer*, int>> triangle_transformations;
	ConstantBuffer triangle_model_transformation;
	TexturedDrawHandler test_square_draw_handler;
	Entity test_square;

	ModelGenerator ground_model_gen;
	Model ground_model;
	int ground_shader_number;
	std::vector<std::pair<ConstantBuffer*, int>> ground_transformations;
	ConstantBuffer ground_model_transformation;
	ModeledDrawHandler ground_draw_handler;
	Entity ground_entity;

	Entity* loaded_entity;
	Entity* left_wall_entity;
};

#endif