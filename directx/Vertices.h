#ifndef __VERTICES_H_INCLUDED__
#define __VERTICES_H_INCLUDED__

#include <vector>

#include "D3D11.h"

#include "Logging.h"

enum VERTEXID {
	TEXTUREVERTEX_ID = 0,
	COLORVERTEX_ID = 1
};

class Vertex {
public:
	Vertex(int number_of_floats);
	Vertex(int number_of_floats, float* all_data);

	void AddVertexBlock(float* new_block, int size);

	float* GetData();
private:
	std::vector<float> data;
	int num_floats;
};

struct TEXTUREVERTEX {
	float x, y, z;
	float u, v;

	static D3D11_INPUT_ELEMENT_DESC input_element_desc[2];
	static int input_element_desc_size;
	static int number_of_elements;
	static VERTEXID vertex_type;
	static int vertex_size;
};

struct COLORVERTEX {
	float x, y, z;
	float r, g, b, a;

	static D3D11_INPUT_ELEMENT_DESC input_element_desc[2];
	static int input_element_desc_size;
	static int number_of_elements;
	static VERTEXID vertex_type;
	static int vertex_size;
};

int VERTEX_SIZE_LOOKUP[];

#endif