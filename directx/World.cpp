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

	// Update the constant buffers in all the render bundles
	for (auto render_bundle_state : current_frame_state.render_bundles) {
		auto dx_buffers = resource_pool->GetRenderBundle(render_bundle_state.first)->GetBuffersForEdit();
		for (int i = 0; i < dx_buffers.size(); i++) {
			DirectX::XMFLOAT4 new_val(render_bundle_state.second.constant_buffers[i].data.data());
			auto cb = dynamic_cast<ConstantBufferTyped<DirectX::XMFLOAT4>*>(dx_buffers[i]);
			cb->GetBufferDataRef() = new_val;
			cb->PushBuffer();
		}
	}

	// Update which entities to display, as well as their positions
	entities_to_display.clear();
	for (int i = 0; i < current_frame_state.entities.size(); i++) {
		EntityState& entity_state = current_frame_state.entities[i];
		if (entity_state.render_bundle_id == 0) {
			Entity* entity_to_update = resource_pool->GetEntity(entity_state.entity_id);
			entities_to_display.push_back(entity_state.entity_id);
			ConstantBufferTyped<TransformationMatrixAndInvTransData>* model_matrix_buffer =
				dynamic_cast<ConstantBufferTyped<TransformationMatrixAndInvTransData>*>(((ModeledDrawHandler*)entity_to_update->GetDrawHandler())->GetConstantBuffers()[0].first);
			DirectX::XMMATRIX model_matrix = DirectX::XMMatrixScaling(entity_state.position.scale[0], entity_state.position.scale[1], entity_state.position.scale[2])*
				DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(entity_state.position.orientation[0], entity_state.position.orientation[1], entity_state.position.orientation[2], entity_state.position.orientation[3]))*
				DirectX::XMMatrixTranslation(entity_state.position.location[0], entity_state.position.location[1], entity_state.position.location[2]);
			model_matrix_buffer->SetBothTransformations(model_matrix);
			model_matrix_buffer->PushBuffer();
		} else if (entity_state.render_bundle_id > 0) {
			// Should prepare the appropriate constant buffers
			
			// This is how the appropriate render bundle state will be gotten,
			// except that its not actually necessary as all the information
			// from it is used during the update
			// RenderBundleState render_bundle = current_frame_state.render_bundles.at(entity_state.render_bundle_id);
			
			//resource_pool->GetRenderBundle(entity_state.render_bundle_id);
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