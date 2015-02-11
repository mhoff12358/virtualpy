#include "ShaderPipeline.h"

ShaderPipeline::ShaderPipeline() {
	
}

void ShaderPipeline::Initialize(ID3D11Device* device, ID3D11DeviceContext* device_context,
	std::string file_name, D3D11_INPUT_ELEMENT_DESC ied[], int ied_size) {

	ID3D10Blob *vs, *ps;
	D3DX11CompileFromFile(file_name.c_str(), 0, 0, "VShader", "vs_5_0", 0, 0, 0, &vs, 0, 0);
	D3DX11CompileFromFile(file_name.c_str(), 0, 0, "PShader", "ps_5_0", 0, 0, 0, &ps, 0, 0);

	device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &vertex_shader);
	device->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), NULL, &pixel_shader);

	device->CreateInputLayout(ied, ied_size, vs->GetBufferPointer(), vs->GetBufferSize(), &input_layout);
}