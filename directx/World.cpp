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
		if (entity_state.render_bundle_id != -1) {
			// Update the entity's information
			Entity* entity_to_update = resource_pool->GetEntity(entity_state.entity_id);
			ConstantBufferTyped<TransformationMatrixAndInvTransData>* model_matrix_buffer =
				dynamic_cast<ConstantBufferTyped<TransformationMatrixAndInvTransData>*>(((ModeledDrawHandler*)entity_to_update->GetDrawHandler())->GetConstantBuffers()[0].first);
			DirectX::XMMATRIX model_matrix = DirectX::XMMatrixScaling(entity_state.position.scale[0], entity_state.position.scale[1], entity_state.position.scale[2])*
				DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(entity_state.position.orientation[0], entity_state.position.orientation[1], entity_state.position.orientation[2], entity_state.position.orientation[3]))*
				DirectX::XMMatrixTranslation(entity_state.position.location[0], entity_state.position.location[1], entity_state.position.location[2]);
			model_matrix_buffer->SetBothTransformations(model_matrix);
			model_matrix_buffer->PushBuffer();
			// Flag the entity to be displayed
			entities_to_display.insert(std::make_pair(entity_state.render_bundle_id, entity_state.entity_id));
		}
	}
}

void World::Draw(RenderMode& render_mode) {
	// 0 is the first current, as 0 is the special ID that maps to no bundle so it is "always active"
	int current_render_bundle_id = 0;
	for (std::pair<const int,int>& render_bundle_entity_id_pair : entities_to_display) {
		if (render_bundle_entity_id_pair.first != current_render_bundle_id) {
			// If we are drawing entities with a new render bundle id, prepare
			// that render bundle and update the currently active bundle id
			current_render_bundle_id = render_bundle_entity_id_pair.first;
			const std::vector<ConstantBuffer*>& buffers = resource_pool->GetRenderBundle(current_render_bundle_id)->GetBuffersForReference();
			for (int i = 0; i < buffers.size(); i++) {
				render_mode.PrepareConstantBuffer(buffers[i], PER_BATCH_CONSTANT_BUFFER_REGISTER + i);
			}
		}
		resource_pool->GetEntity(render_bundle_entity_id_pair.second)->Draw(render_mode);
	}
}

std::array<float, 3> World::GetPlayerLocation() {
	return player_location;
}

std::array<float, 4> World::GetPlayerOrientation() {
	return player_orientation;
}