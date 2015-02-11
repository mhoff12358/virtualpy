#pragma once

#include "Windows.h"

#include <iostream>
#include <string>
#include "DirectXMath.h"

#include "../MainLoop.h"

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


class DirectxLoop : public MainLoop {
public:
	DirectxLoop(bool uo, std::string rl, FrameStateInterpolater* ss);

	void Begin();

protected:
	bool use_oculus;
	std::string resource_location;
};
