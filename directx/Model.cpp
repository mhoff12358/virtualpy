#include "Model.h"


void Model::CleanupVertexBuffer() {
	vertex_buffer->Release();
}

ID3D11Buffer* Model::GetVertexBuffer() const {
	return vertex_buffer;
}
