#ifndef __CONSTANT_BUFFER_H_INCLUDED__
#define __CONSTANT_BUFFER_H_INCLUDED__

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include "DirectXMath.h"

struct ConstantBufferData {
	DirectX::XMFLOAT4X4 transformation;
};

class ConstantBuffer {
public:
	ConstantBuffer();
	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con);

	void CreateBuffer();
	void PushBuffer();

	void ActivateBuffer(int buffer_register);

	ConstantBufferData& GetBufferData();

private:
	ID3D11Device* device_interface;
	ID3D11DeviceContext* device_context;

	ConstantBufferData buffer_data;
	ID3D11Buffer* const_buffer;
};

#endif