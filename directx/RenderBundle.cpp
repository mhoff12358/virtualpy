#include "RenderBundle.h"

RenderBundle::RenderBundle() {

}

RenderBundle::RenderBundle(ID3D11Device* dev, ID3D11DeviceContext* dev_con, int num_constant_buffers) {
	buffers.resize(num_constant_buffers);
	for (int i = 0; i < num_constant_buffers; i++) {
		buffers[i] = new ConstantBufferTyped<DirectX::XMFLOAT4>;
		buffers[i]->Initialize(dev, dev_con);
		buffers[i]->CreateBuffer();
	}
}

const std::vector<ConstantBuffer*>& RenderBundle::GetBuffersForReference() {
	return buffers;
}

std::vector<ConstantBuffer*>& RenderBundle::GetBuffersForEdit() {
	return buffers;
}
