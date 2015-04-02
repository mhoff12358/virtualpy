#include "World.h"

World::World(ViewState* vs) : view_state(vs) {
};

void World::Initialize(InputHandler* ih, DXResourcePool* dxrp) {
	input_handler = ih;
	resource_pool = dxrp;

	player_location = { { 0.0f, 0.0f, 0.0f } };
	player_orientation = { { 0.0f, 0.0f, 0.0f, 0.0f } };
}

void World::UpdateLogic(int time_delta) {
	if (input_handler->IsOculusActive()) {
		ovrPosef head_pose = input_handler->GetHeadPose().ThePose;
		player_orientation = OculusHelper::ConvertQuaternionToArray(head_pose.Orientation);
	}

	FrameState current_frame_state = input_handler->GetFrameState();

	player_location = current_frame_state.camera_position.location;

	entities_to_display.clear();
	for (int i = 0; i < current_frame_state.entities.size(); i++) {
		EntityState& entity_state = current_frame_state.entities[i];
		if (entity_state.display_state == 1) {
			Entity* entity_to_update = resource_pool->GetEntity(entity_state.entity_id);
			entities_to_display.push_back(entity_state.entity_id);
			ConstantBuffer* model_matrix_buffer = ((ModeledDrawHandler*)entity_to_update->GetDrawHandler())->GetConstantBuffers().front().first;
			ConstantBuffer* model_matrix_inv_trans_buffer = ((ModeledDrawHandler*)entity_to_update->GetDrawHandler())->GetConstantBuffers()[1].first;
			DirectX::XMMATRIX model_matrix = DirectX::XMMatrixScaling(entity_state.position.scale[0], entity_state.position.scale[1], entity_state.position.scale[2])*
				DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(entity_state.position.orientation[0], entity_state.position.orientation[1], entity_state.position.orientation[2], entity_state.position.orientation[3]))*
				DirectX::XMMatrixTranslation(entity_state.position.location[0], entity_state.position.location[1], entity_state.position.location[2]);
			DirectX::XMStoreFloat4x4(
				&dynamic_cast<ConstantBufferTyped<TransformationMatrixData>*>(model_matrix_buffer)->GetBufferDataRef().transformation,
				model_matrix);
			DirectX::XMStoreFloat4x4(
				&dynamic_cast<ConstantBufferTyped<TransformationMatrixData>*>(model_matrix_inv_trans_buffer)->GetBufferDataRef().transformation,
				DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(NULL, model_matrix)));
			model_matrix_buffer->PushBuffer();
			model_matrix_inv_trans_buffer->PushBuffer();
		}
	}
}

void World::Draw(RenderMode& render_mode) {
	for (int& entity_id_to_draw : entities_to_display) {
		resource_pool->GetEntity(entity_id_to_draw)->Draw(render_mode);
	}
}

std::array<float, 3> World::GetPlayerLocation() {
	return player_location;
}

std::array<float, 4> World::GetPlayerOrientation() {
	return player_orientation;
}