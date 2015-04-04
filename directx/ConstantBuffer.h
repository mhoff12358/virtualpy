#ifndef __CONSTANT_BUFFER_H_INCLUDED__
#define __CONSTANT_BUFFER_H_INCLUDED__

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include "DirectXMath.h"

struct TransformationMatrixData {
	DirectX::XMFLOAT4X4 transformation;
};

struct TransformationMatrixAndInvTransData {
	DirectX::XMFLOAT4X4 transformation;
	DirectX::XMFLOAT4X4 transformation_inv_trans;
};

class ConstantBuffer {
public:
	ConstantBuffer();
	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con);

	void CreateBuffer();
	void PushBuffer();

	void ActivateBuffer(int buffer_register);

	virtual void* GetBufferData() = 0;
	virtual unsigned int GetBufferDataSize() = 0;

protected:
	ID3D11Device* device_interface;
	ID3D11DeviceContext* device_context;

	ID3D11Buffer* const_buffer;
};

template <typename ConstantBufferData>
class ConstantBufferTypedTemp : public ConstantBuffer {
public:
	void* GetBufferData();
	unsigned int GetBufferDataSize();
	ConstantBufferData& GetBufferDataRef();

protected:
	ConstantBufferData buffer_data;
};

template <typename ConstantBufferData>
ConstantBufferData& ConstantBufferTypedTemp<ConstantBufferData>::GetBufferDataRef() {
	return buffer_data;
}

template <typename ConstantBufferData>
unsigned int ConstantBufferTypedTemp<ConstantBufferData>::GetBufferDataSize() {
	return sizeof(ConstantBufferData);
}

template <typename ConstantBufferData>
void* ConstantBufferTypedTemp<ConstantBufferData>::GetBufferData() {
	return (void*)&buffer_data;
}

template <typename ConstantBufferData>
class ConstantBufferTyped : public ConstantBufferTypedTemp < ConstantBufferData > {

};

template <>
class ConstantBufferTyped<TransformationMatrixData> : public ConstantBufferTypedTemp<TransformationMatrixData> {
public:
	void SetTransformation(DirectX::XMMATRIX new_transformation);
};

template <>
class ConstantBufferTyped<TransformationMatrixAndInvTransData> : public ConstantBufferTypedTemp<TransformationMatrixAndInvTransData>{
public:
	void SetBothTransformations(DirectX::XMMATRIX new_transformation);
	void SetTransformation(DirectX::XMMATRIX new_transformation);
	void SetTransformationInvTrans(DirectX::XMMATRIX new_transformation_inv_trans);
};

#endif