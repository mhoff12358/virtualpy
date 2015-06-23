#include "VRBackendTranslator.h"

namespace virtualpy {

	DirectX::XMMATRIX VRBackendTranslator::PositionStateToMatrix(const PositionState& position_state) {
		return DirectX::XMMatrixMultiply(DirectX::XMMatrixMultiply(
			DirectX::XMMatrixTranslation(position_state.location[0], position_state.location[1], position_state.location[2]),
			DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(position_state.orientation[0], position_state.orientation[1], position_state.orientation[2], position_state.orientation[3]))),
			DirectX::XMMatrixScaling(position_state.scale[0], position_state.scale[1], position_state.scale[2]));
	}

} // virtualpy