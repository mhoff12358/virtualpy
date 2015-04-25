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
	Model(ID3D11Buffer* verbuf, UINT str, UINT off, UINT nvert, D3D_PRIMITIVE_TOPOLOGY p_type) :
		vertex_buffer(verbuf), stride(str), offset(off), num_vertices(nvert), primitive_type(p_type) {}

	void CleanupVertexBuffer();

	ID3D11Buffer* GetVertexBuffer() const;

	UINT stride;
	UINT offset;
	UINT num_vertices;
	D3D_PRIMITIVE_TOPOLOGY primitive_type;

private:
	ID3D11Buffer* vertex_buffer;
};

class ModelGenerator {
public:
	ModelGenerator(VertexType v_type, D3D_PRIMITIVE_TOPOLOGY p_type);
	virtual ~ModelGenerator() {};

	void AddVertex(Vertex new_vertex);
	void AddVertexBatch(std::vector<Vertex>& new_vertexes);

	Model DumpModel(ID3D11Device* device, ID3D11DeviceContext* device_context);

private:
	std::vector<Vertex> vertices;
	ID3D11Buffer* vertex_buffer;
	VertexType vertex_type;
	D3D_PRIMITIVE_TOPOLOGY primitive_type;
	
	void InitializeVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* device_context);
};

#endif