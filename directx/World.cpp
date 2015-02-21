#include "World.h"

World::World(ViewState* vs) : view_state(vs), texture(true, false), triangle_gen(TEXTUREVERTEX::vertex_size), ground_model_gen(COLORVERTEX::vertex_size) {
};

void World::Initialize(InputHandler* ih, DXResourcePool* dxrp) {
	input_handler = ih;
	resource_pool = dxrp;

	player_location = { { 0.0f, 0.0f, 0.0f } };
	player_orientation = { { 0.0f, 0.0f, 0.0f, 0.0f } };

	TEXTUREVERTEX new_vertex = { -1.0f, 1.0f, -1.0f, 0.0f, 0.0f };
	triangle_gen.AddVertex(&new_vertex);
	new_vertex = { 1.0f, 1.0f, -1.0f, 1.0f, 0.0f };
	triangle_gen.AddVertex(&new_vertex);
	new_vertex = { -1.0f, -1.0f, -1.0f, 0.0f, 1.0f };
	triangle_gen.AddVertex(&new_vertex);
	new_vertex = { 1.0f, -1.0f, -1.0f, 1.0f, 1.0f };
	triangle_gen.AddVertex(&new_vertex);
	triangle = triangle_gen.DumpModel(view_state->device_interface, view_state->device_context);

	texture.Initialize(view_state->device_interface, view_state->device_context, std::array<int, 2>{ {128, 128} });
	texture.Fill();

	shader_number = 0;

	triangle_model_transformation.Initialize(view_state->device_interface, view_state->device_context);
	XMStoreFloat4x4(&(triangle_model_transformation.GetBufferData().transformation), DirectX::XMMatrixIdentity());
	triangle_model_transformation.CreateBuffer();
	triangle_transformations.push_back(std::make_pair(&triangle_model_transformation, 1));

	test_square_draw_handler.Initialize(&shader_number, triangle_transformations, &triangle, &texture, 0, 0);
	test_square.Initialize(&test_square_draw_handler);

	COLORVERTEX new_color_vertex;
	new_color_vertex = { -1.0f, 1.0f, -1.0f, 0.573f, 0.434f, 0.355f, 1.0f };
	ground_model_gen.AddVertex(&new_color_vertex);
	new_color_vertex = { 1.0f, 1.0f, -1.0f, 0.573f, 0.434f, 0.355f, 1.0f };
	ground_model_gen.AddVertex(&new_color_vertex);
	new_color_vertex = { -1.0f, 1.0f, 1.0f, 0.573f, 0.434f, 0.355f, 1.0f };
	ground_model_gen.AddVertex(&new_color_vertex);
	new_color_vertex = { 1.0f, 1.0f, 1.0f, 0.573f, 0.434f, 0.355f, 1.0f };
	ground_model_gen.AddVertex(&new_color_vertex);
	ground_model = ground_model_gen.DumpModel(view_state->device_interface, view_state->device_context);
	ground_shader_number = 1;

	ground_model_transformation.Initialize(view_state->device_interface, view_state->device_context);
	XMStoreFloat4x4(&(ground_model_transformation.GetBufferData().transformation), DirectX::XMMatrixTranslation(0, -2, 0));
	ground_model_transformation.CreateBuffer();
	ground_transformations.push_back(std::make_pair(&ground_model_transformation, 1));

	ground_draw_handler.Initialize(&ground_shader_number, ground_transformations, &ground_model);
	ground_entity.Initialize(&ground_draw_handler);

	//loaded_entity = EntityFactory::GetInstance().LoadEntityFromFile("C:\\Users\\Matt\\Desktop\\TerrainButcher\\Project2\\ground.mod");
	//left_wall_entity = EntityFactory::GetInstance().LoadEntityFromFile("C:\\Users\\Matt\\Desktop\\TerrainButcher\\Project2\\left_wall.mod");
}

void World::UpdateLogic(int time_delta) {
	if (input_handler->IsOculusActive()) {
		ovrPosef head_pose = input_handler->GetHeadPose().ThePose;
		player_orientation = OculusHelper::ConvertQuaternionToArray(head_pose.Orientation);
	}

	FrameState current_frame_state = input_handler->GetFrameState();
	entities_to_display.clear();
	for (int i = 0; i < current_frame_state.entities.size(); i++) {
		EntityState& entity_state = current_frame_state.entities[i];
		if (entity_state.display_state == 1) {
			Entity* entity_to_update = resource_pool->GetEntity(entity_state.entity_id);
			entities_to_display.push_back(entity_state.entity_id);
			ConstantBuffer* model_matrix_buffer = ((ModeledDrawHandler*)entity_to_update->GetDrawHandler())->GetConstantBuffers().front().first;
			DirectX::XMStoreFloat4x4(
				&model_matrix_buffer->GetBufferData().transformation,
				DirectX::XMMatrixScaling(entity_state.scale[0], entity_state.scale[1], entity_state.scale[2])*
				DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(entity_state.orientation[0], entity_state.orientation[1], entity_state.orientation[2], entity_state.orientation[3]))*
				DirectX::XMMatrixTranslation(entity_state.location[0], entity_state.location[1], entity_state.location[2]));
			model_matrix_buffer->PushBuffer();
		}
	}
}

void World::Draw(RenderMode& render_mode) {
	//render_mode.PrepareConstantBuffer(&camera_transformation, 0);
	//test_square.Draw(render_mode);
	//ground_entity.Draw(render_mode);
	for (int& entity_id_to_draw : entities_to_display) {
		resource_pool->GetEntity(entity_id_to_draw)->Draw(render_mode);
	}
	//loaded_entity->Draw(render_mode);
	//left_wall_entity->Draw(render_mode);
}

std::array<float, 3> World::GetPlayerLocation() {
	return player_location;
}

std::array<float, 4> World::GetPlayerOrientation() {
	return player_orientation;
}