#pragma once

#include <D3D11.h>
#include <DirectXMath.h>

#include "FrameState.h"

namespace virtualpy {
	class VRBackendTranslator
	{
	public:
		static DirectX::XMMATRIX PositionStateToMatrix(const PositionState& position_state);
	};
} // virtualpy

