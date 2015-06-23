#include "VRBackendMainLoop.h"

namespace virtualpy {
	VRBackendMainLoop::VRBackendMainLoop(bool in_use_oculus, std::string in_resources_location, ResourcePool** in_resource_pool_location, FrameStateInterpolater* ss, IOStateBuffer* iosb)
		: MainLoop(ss, iosb), use_oculus(in_use_oculus), resources_location(in_resources_location), resource_pool_location(in_resource_pool_location)
	{
		frame_state_buffer = state_source->frame_state_buffer;
	}

	VRBackendMainLoop::VRBackendMainLoop() : MainLoop(NULL, NULL) {

	}


	void VRBackendMainLoop::Begin() {
		std::mutex preparation_mutex;
		std::condition_variable preparation_cv;
		std::unique_lock<std::mutex> preparation_lock(preparation_mutex);
		preparation_cv.wait(preparation_lock);
		BeginWithPrep(&preparation_mutex, &preparation_cv);
	}

	void VRBackendMainLoop::BeginWithPrep(std::mutex* preparation_mutex, std::condition_variable* preparation_cv) {
		MSG msg;
		int prev_time = timeGetTime();
		int frame_index = 0;

		vr_backend = BeginDirectx(use_oculus, resources_location);
		actual_resource_pool.Initialize(vr_backend.resource_pool, vr_backend.entity_handler);
		*resource_pool_location = &actual_resource_pool;
		std::unique_lock<std::mutex> preparation_lock(*preparation_mutex);
		preparation_cv->notify_all();
		preparation_lock.unlock();

		std::thread frame_state_check_thread(&VRBackendMainLoop::CheckForFrameStates, this);

		/*
		ConstantBufferTyped<TransformationMatrixAndInvTransData>* object_settings = new ConstantBufferTyped<TransformationMatrixAndInvTransData>(CB_PS_VERTEX_SHADER);
		object_settings->CreateBuffer(vr_backend.view_state->device_interface);
		object_settings->SetBothTransformations(DirectX::XMMatrixTranslation(0, 0, -4));
		object_settings->PushBuffer(vr_backend.view_state->device_context);

		vr_backend.entity_handler->AddEntity(Entity(
			ES_DISABLED,
			vr_backend.resource_pool->LoadPixelShader("objshader.hlsl"),
			vr_backend.resource_pool->LoadVertexShader("objshader.hlsl", ObjLoader::vertex_type.GetVertexType(), ObjLoader::vertex_type.GetSizeVertexType()),
			ShaderSettings(NULL),
			vr_backend.resource_pool->LoadModel("C:\\Users\\Matt\\Desktop\\rhod.obj"),
			object_settings));
		*/

		vr_backend.entity_handler->FinishUpdate();

		while (TRUE)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				if (msg.message == WM_QUIT) {
					break;
				}
				else if (msg.message == WM_KEYDOWN) {
					vr_backend.input_handler->HandleKeydown(msg.wParam);
				}
			}

			int new_time = timeGetTime();
			int time_delta = new_time - prev_time;
			prev_time = new_time;

			vr_backend.input_handler->UpdateStates(frame_index);
			vr_backend.world->UpdateLogic(time_delta);

			vr_backend.render_pipeline->Render();

			++frame_index;
		}

		// clean up DirectX and COM
		vr_backend.view_state->Cleanup();
		if (vr_backend.input_handler->IsOculusActive()) {
			vr_backend.oculus->Cleanup();
		}
	}

	void VRBackendMainLoop::SignalFrameStateUpdate() {
		std::unique_lock<std::mutex> fsb_check_lock(fsb_check_mutex);
		fsb_check_cond.notify_one();
		fsb_check_lock.unlock();
	}

	void VRBackendMainLoop::CheckForFrameStates() {
		std::unique_lock<std::mutex> fsb_check_lock2(fsb_check_mutex);
		//fsb_check_lock.unlock();
		while (true) { // While loop to handle spurious wakeups
			if (frame_state_buffer->GetNumberOfStates() >= 1) {
				break;
			}
			fsb_check_cond.wait(fsb_check_lock2);
		}
		fsb_check_lock2.unlock();
		fsb_check_lock2.release();

		while (true) {
			auto upcoming_state_iter = frame_state_buffer->GetFirstState();
			// Set the position of all entities based on the upcoming timed state (S_n+1)
			SetEntityPositions(upcoming_state_iter->second);

			bool velocities_zeroed = false;
			bool velocities_correctly_set = false;
			// Wait for it to be S_n+1's time, or for S_n+2 to appear if it hasn't already
			
			// Now wait until another state is added
			std::unique_lock<std::mutex> fsb_check_lock(fsb_check_mutex);
			while (true) { // While loop to handle spurious wakeups
				if (frame_state_buffer->GetNumberOfStates() >= 2) {
					break;
				}
				fsb_check_cond.wait(fsb_check_lock);
			}
			fsb_check_lock.unlock();

			// No spurious call happened to the condition variable, so S_n+2 now exists
			// Set the velocities correctly
			velocities_correctly_set = true;

			// Wait until it is time to display S_n+1
			//ConstantBufferTyped<TransformationMatrixAndInvTransData>* obj_set = vr_backend.entity_handler->GetEntityObjectSettings<TransformationMatrixAndInvTransData>(0);
			//obj_set->SetBothTransformations(DirectX::XMMatrixTranslation(0, 0, -4));
			vr_backend.entity_handler->FinishUpdate();
			frame_state_buffer->PopState();
		}
	}

	void VRBackendMainLoop::SetEntityPositions(const FrameState& frame_state) {
		for (const std::pair<int, EntityState> entity_state_and_num : frame_state.entities) {
			const EntityState& entity_state = entity_state_and_num.second;
			ConstantBufferTyped<TransformationMatrixAndInvTransData>* object_settings = vr_backend.entity_handler->GetEntityObjectSettings<TransformationMatrixAndInvTransData>(entity_state.entity_id);

			object_settings->SetBothTransformations(VRBackendTranslator::PositionStateToMatrix(entity_state.position));
		}
	}
}