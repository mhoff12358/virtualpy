#pragma once

#include "Windows.h"

#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include "DirectXMath.h"

#include "../MainLoop.h"
#include "../FrameState.h"

#include "ViewState.h"
#include "RenderMode.h"
#include "Model.h"
#include "ConstantBuffer.h"
#include "OrthoCamera.h"
#include "InputHandler.h"
#include "World.h"
#include "Logging.h"
#include "Vertices.h"
#include "RenderingPipeline.h"
#include "DXResourcePool.h"


class DirectxLoop : public MainLoop {
public:
	DirectxLoop(bool uo, std::string rl, DXResourcePool* dxrp, FrameStateInterpolater* ss);

	void Begin();
	void BeginWithPrep(std::mutex* preparation_mutex, std::condition_variable* preparation_cv);

protected:
	bool use_oculus;
	std::string resource_location;
	DXResourcePool* resource_pool;
};
