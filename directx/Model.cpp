#include "Model.h"


void Model::CleanupVertexBuffer() {
	vertex_buffer->Release();
}

ID3D11Buffer* Model::GetVertexBuffer() const {
	return vertex_buffer;
}

ModelGenerator::ModelGenerator(VertexType v_type) : vertex_type(v_type) {

}

void ModelGenerator::AddVertex(Vertex new_vertex) {
	//if (new_vertex.GetVertexType() != vertex_type) {
	//	OutputFormatted("Attempting to add a vertex of the wrong type");
	//	return;
	//}
	vertices.push_back(new_vertex);
}

void ModelGenerator::InitializeVertexBuffer(ID3D11Device* device, ID3D11DeviceContext* device_context) {
	D3D11_BUFFER_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(buffer_desc));

	buffer_desc.Usage = D3D11_USAGE_DYNAMIC; // Should be made configurable
	buffer_desc.ByteWidth = vertices.size();
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertices_data;
	int vertex_data_size = vertex_type.GetVertexSize();
	char* vertices_raw_data = new char[vertex_data_size*vertices.size()];
	for (int i = 0; i < vertices.size(); i++) {
		memcpy((void*)(vertices_raw_data + i*vertex_data_size), (void*)vertices[i].GetData(), vertex_data_size);
	}
	vertices_data.pSysMem = (void*)vertices_raw_data;

	device->CreateBuffer(&buffer_desc, &vertices_data, &vertex_buffer);

	/*D3D11_MAPPED_SUBRESOURCE ms;
	device_context->Map(vertex_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices.data(), sizeof(VERTEX) * vertices.size());
	device_context->Unmap(vertex_buffer, NULL);*/
}

Model ModelGenerator::DumpModel(ID3D11Device* device, ID3D11DeviceContext* device_context) {
	InitializeVertexBuffer(device, device_context);
	Model model(vertex_buffer, vertex_type.GetVertexSize(), 0, vertices.size());
	return model;
}