#include "Vertices.h"

Vertex::Vertex(int number_of_floats) {
	num_floats = number_of_floats;
	data.resize(num_floats);
}

Vertex::Vertex(int number_of_floats, float* all_data) {
	num_floats = number_of_floats;
	data.resize(num_floats);

	memcpy(data.data(), all_data, sizeof(float)*num_floats);
}

void Vertex::AddVertexBlock(float* new_block, int size) {
	int current_size = data.size();
	if (size + current_size <= num_floats) {
		data.resize(size + current_size);
		memcpy(data.data() + current_size, new_block, size);
	} else {
		OutputFormatted("Error attempting to add too much data to a vertex");
	}
}

float* Vertex::GetData() {
	if (data.size() != num_floats) {
		OutputFormatted("Error attempting to get a vertex with the wrong amount of data");
		return NULL;
	}
	return data.data();
}

D3D11_INPUT_ELEMENT_DESC TEXTUREVERTEX::input_element_desc[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
int TEXTUREVERTEX::input_element_desc_size = 2;
int TEXTUREVERTEX::number_of_elements = 5;
VERTEXID TEXTUREVERTEX::vertex_type = TEXTUREVERTEX_ID;
int TEXTUREVERTEX::vertex_size = 5 * sizeof(float);


D3D11_INPUT_ELEMENT_DESC COLORVERTEX::input_element_desc[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

int COLORVERTEX::input_element_desc_size = 2;
int COLORVERTEX::number_of_elements = 7;
VERTEXID COLORVERTEX::vertex_type = COLORVERTEX_ID;
int COLORVERTEX::vertex_size = 7 * sizeof(float);

int VERTEX_SIZE_LOOKUP[2] = { TEXTUREVERTEX::number_of_elements,
COLORVERTEX::number_of_elements };