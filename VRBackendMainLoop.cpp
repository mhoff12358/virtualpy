#include "VRBackendMainLoop.h"

namespace virtualpy {
	VRBackendMainLoop::VRBackendMainLoop(bool in_use_oculus, std::string in_resources_location, ResourcePool** in_resource_pool_location, FrameStateInterpolater* ss, IOStateBuffer* iosb)
		: MainLoop(ss, iosb), use_oculus(in_use_oculus), resources_location(in_resources_location), resource_pool_location(in_resource_pool_location)
	{

	}


	void VRBackendMainLoop::Begin() {
		std::mutex preparation_mutex;
		std::condition_variable preparation_cv;
		std::unique_lock<std::mutex> preparation_lock(preparation_mutex);
		preparation_cv.wait(preparation_lock);
		BeginWithPrep(&preparation_mutex, &preparation_cv);
	}

	void VRBackendMainLoop::BeginWithPrep(std::mutex* preparation_mutex, std::condition_variable* preparation_cv) {

	}

}