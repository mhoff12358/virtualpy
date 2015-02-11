#ifndef __RENDER_MODE_H_INCLUDED__
#define __RENDER_MODE_H_INCLUDED__

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include <string>
#include <vector>
#include <array>
#include "Model.h"
#include "ShaderPipeline.h"
#include "ConstantBuffer.h"

class RenderMode {
public:
	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con, ID3D11RenderTargetView* rend_tar_view, ID3D11DepthStencilView* dep_sten_view, D3D11_VIEWPORT view);
	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con, ID3D11RenderTargetView* rend_tar_view, ID3D11DepthStencilView* dep_sten_view);
	void SetViewport(D3D11_VIEWPORT view);
	void SetViewport(std::array<int, 2> offset, std::array<int, 2> size);
	void SetViewport(std::array<int, 2> size);
	void SetViewport(int xoff, int yoff, int width, int height);
	void SetViewport(int width, int height);
	void AddShader(std::string file_name, D3D11_INPUT_ELEMENT_DESC ied[], int ied_size);

	void Cleanup();

	void Prepare();
	void PrepareShader(int shader_number);
	void PrepareConstantBuffer(ConstantBuffer* constant_buffer, int buffer_slot);

	void Clear(D3DXCOLOR color);

	void RenderModel(const Model& model) const;

private:
	ID3D11Device* device;
	ID3D11DeviceContext* device_context;

	ID3D11RenderTargetView* render_target_view;
	ID3D11DepthStencilView* depth_stencil_view;
	D3D11_VIEWPORT viewport;

	std::vector<ShaderPipeline> shaders;
};

#endif