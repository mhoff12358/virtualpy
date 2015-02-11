#ifndef __MODEL_H_INCLUDED__
#define __MODEL_H_INCLUDED__

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include <vector>

#include "Vertices.h"

class Model {
public:
	Model() {};
	Model(ID3D11Buffer* verbuf, UINT str, UINT off, UINT nvert) :
		vertex_buffer(verbuf), stride(str), offset(off), num_vertices(nvert) {}

	void CleanupVertexBuffer();

	ID3D11Buffer* GetVertexBuffer() const;

	UINT stride;
	UINT offset;
	UINT num_vertices;

private:
	ID3D11Buffer* vertex_buffer;
};

template <class VERTEX>
class ModelGenerator {
public:
	void AddVertex(const VERTEX& new_vertex);

	Model DumpModel(ID3D11Device* device, ID3D11DeviceContext* device_context);

private:
	std::vector<VERTEX> vertices;
	ID3D11Buffer* vertex_buffer;
	
	void InitializeVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* device_context);
};


template <class VERTEX>
void ModelGenerator<VERTEX>::AddVertex(const VERTEX& new_vertex) {
	vertices.push_back(new_vertex);
}

template <class VERTEX>
void ModelGenerator<VERTEX>::InitializeVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* device_context) {
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(buffer_desc));

	buffer_desc.Usage = D3D11_USAGE_DYNAMIC; // Should be made configurable
	buffer_desc.ByteWidth = sizeof(VERTEX) * vertices.size();
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	device->CreateBuffer(&buffer_desc, NULL, &vertex_buffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	device_context->Map(vertex_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices.data(), sizeof(VERTEX) * vertices.size());
	device_context->Unmap(vertex_buffer, NULL);
}

template <class VERTEX>
Model ModelGenerator<VERTEX>::DumpModel(ID3D11Device* device, ID3D11DeviceContext* device_context) {
	InitializeVertexBuffer(device, device_context);
	Model model(vertex_buffer, sizeof(VERTEX), 0, vertices.size());
	return model;
}

#endif