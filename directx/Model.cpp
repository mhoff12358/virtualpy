#include "Model.h"


void Model::CleanupVertexBuffer() {
	vertex_buffer->Release();
}

ID3D11Buffer* Model::GetVertexBuffer() const {
	return vertex_buffer;
}

ModelGenerator::ModelGenerator(int v_size) : vertex_size(v_size) {

}

void ModelGenerator::AddVertex(void* new_vertex) {
	int original_size = vertices.size();
	vertices.resize(original_size + vertex_size);
	memcpy(vertices.data() + original_size, new_vertex, vertex_size);
}

void ModelGenerator::InitializeVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* device_context) {
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(buffer_desc));

	buffer_desc.Usage = D3D11_USAGE_DYNAMIC; // Should be made configurable
	buffer_desc.ByteWidth = vertices.size();
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertices_data;
	vertices_data.pSysMem = vertices.data();

	device->CreateBuffer(&buffer_desc, &vertices_data, &vertex_buffer);

	/*D3D11_MAPPED_SUBRESOURCE ms;
	device_context->Map(vertex_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices.data(), sizeof(VERTEX) * vertices.size());
	device_context->Unmap(vertex_buffer, NULL);*/
}

Model ModelGenerator::DumpModel(ID3D11Device* device, ID3D11DeviceContext* device_context) {
	InitializeVertexBuffer(device, device_context);
	Model model(vertex_buffer, vertex_size, 0, vertices.size()/vertex_size);
	return model;
}