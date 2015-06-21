#pragma once

#include <Windows.h>

#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>

#include "MainLoop.h"
#include "ResourcePool.h"
#include "VRBackendResourcePool.h"
#include "VRBackend/BeginDirectx.h"

namespace virtualpy {

	class VRBackendMainLoop : public MainLoop {
	public:
		VRBackendMainLoop(bool in_use_oculus, std::string in_resources_location, ResourcePool** in_resource_pool_location, FrameStateInterpolater* ss, IOStateBuffer* iosb);
		VRBackendMainLoop();

		void Begin();
		void BeginWithPrep(std::mutex* preparation_mutex, std::condition_variable* preparation_cv);

		void SignalFrameStateUpdate();

		void SetEntityPositions(const FrameState& frame_state);

	private:
		void CheckForFrameStates();

		bool use_oculus;
		std::string resources_location;
		ResourcePool** resource_pool_location;
		VRBackendBasics vr_backend;
		VRBackendResourcePool actual_resource_pool;

		FrameStateBuffer* frame_state_buffer;

		std::mutex fsb_check_mutex;
		std::condition_variable fsb_check_cond;
	};

} // virtualpy