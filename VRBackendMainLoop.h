#pragma once

#include <mutex>
#include <condition_variable>

#include "MainLoop.h"
#include "VRBackend/BeginDirectx.h"

namespace virtualpy {

	class VRBackendMainLoop : MainLoop {
	public:
		VRBackendMainLoop(bool in_use_oculus, std::string in_resources_location, ResourcePool** in_resource_pool_location, FrameStateInterpolater* ss, IOStateBuffer* iosb);

		void Begin();
		void BeginWithPrep(std::mutex* preparation_mutex, std::condition_variable* preparation_cv);

	private:
		bool use_oculus;
		std::string resources_location;
		ResourcePool** resource_pool_location;
	};

} // virtualpy