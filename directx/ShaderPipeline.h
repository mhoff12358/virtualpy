#ifndef __SHADER_PIPELINE_H_INCLUDED__
#define __SHADER_PIPELINE_H_INCLUDED__

#include <string>

#include "D3D11.h"
#include <d3dx11.h>
#include <d3dx10.h>

class ShaderPipeline {
public:
	ShaderPipeline();

	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con,
		std::string file_name, D3D11_INPUT_ELEMENT_DESC ied[], int ied_size);

	ID3D11InputLayout* input_layout;
	ID3D11VertexShader* vertex_shader;
	ID3D11PixelShader* pixel_shader;
};

#endif