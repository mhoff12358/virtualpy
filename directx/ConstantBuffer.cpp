#include "ConstantBuffer.h"

ConstantBuffer::ConstantBuffer() {}


void ConstantBuffer::Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con) {
	device_interface = dev;
	device_context = dev_con;
}

void ConstantBuffer::CreateBuffer() {
	// Description of the buffer being created
	// Must be set to a constant buffer
	D3D11_BUFFER_DESC const_buff_desc;
	const_buff_desc.ByteWidth = sizeof(ConstantBufferData);
	const_buff_desc.Usage = D3D11_USAGE_DYNAMIC;
	const_buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	const_buff_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	const_buff_desc.MiscFlags = 0;
	const_buff_desc.StructureByteStride = 0;

	// Create the subresource data that carries the buffer data
	D3D11_SUBRESOURCE_DATA sub_data;
	sub_data.pSysMem = &buffer_data;
	sub_data.SysMemPitch = 0;
	sub_data.SysMemSlicePitch = 0;

	device_interface->CreateBuffer(&const_buff_desc, &sub_data, &const_buffer);
}

void ConstantBuffer::PushBuffer() {
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// Disables GPU access to the data
	device_context->Map(const_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, &buffer_data, sizeof(buffer_data));
	device_context->Unmap(const_buffer, 0);
}

void ConstantBuffer::ActivateBuffer(int buffer_register) {
	device_context->VSSetConstantBuffers(buffer_register, 1, &const_buffer);
}

ConstantBufferData& ConstantBuffer::GetBufferData() {
	return buffer_data;
}