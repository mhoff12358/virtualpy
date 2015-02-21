#include "Vertices.h"

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