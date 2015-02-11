#ifndef __TEXTURE_H_INCLUDED__
#define __TEXTURE_H_INCLUDED__

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include <array>

#include "Logging.h"

class Texture {
public:
	Texture(bool sr, bool rt) : is_shader_resource(sr), is_render_target(rt) {}

	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con, std::array<int, 2> size);
	void Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con, ID3D11Texture2D* existing_texture, std::array<int, 2> size);

	void Cleanup();

	void SetAsResource(int texture_num, int sampler_num);
	void UnsetAsResource();
	void Fill();

	ID3D11Texture2D* GetTexture();
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11ShaderResourceView* GetShaderResourceView();

protected:
	virtual void CreateResources(bool generate_texture);
	virtual void CreateTexture();
	virtual void CreateShaderResourceView();
	virtual void CreateSampler();
	virtual void CreateRenderTargetView();

	std::array<int, 2> size;
	bool owns_texture = false;
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* shader_resource_view;
	ID3D11SamplerState* sampler;
	ID3D11RenderTargetView* render_target_view;

	// Things injected
	ID3D11Device* device_interface;
	ID3D11DeviceContext* device_context;

	// The resource numbers for this texture and sampler
	// (set when this is made active in the shader, cleared to -1 when unset)
	int texture_number = -1;
	int sampler_number = -1;

	// Functionality state, shouldn't change once initalized
	bool is_shader_resource;
	bool is_render_target;
};

class DepthTexture : public Texture {
public:
	DepthTexture() : Texture(false, false) {}
	virtual void CreateTexture();
};

#endif