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

class ModelGenerator {
public:
	ModelGenerator(int v_size);

	void AddVertex(void* new_vertex);
	void SetVertexSize();

	Model DumpModel(ID3D11Device* device, ID3D11DeviceContext* device_context);

private:
	std::vector<char> vertices;
	ID3D11Buffer* vertex_buffer;
	int vertex_size;
	
	void InitializeVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* device_context);
};



#endif