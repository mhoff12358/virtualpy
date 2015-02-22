#include "Texture.h"

void Texture::Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con, std::array<int, 2> new_size) {
	device_interface = dev;
	device_context = dev_con;

	size = new_size;

	CreateResources(true);
}

void Texture::Initialize(ID3D11Device* dev, ID3D11DeviceContext* dev_con, ID3D11Texture2D* existing_texture, std::array<int, 2> new_size) {
	device_interface = dev;
	device_context = dev_con;

	size = new_size;

	texture = existing_texture;

	CreateResources(false);
}

void Texture::Cleanup() {
	// IMPLEMENT THIS!
}

void Texture::CreateResources(bool generate_texture) {
	if (generate_texture) {
		CreateTexture();
	}
	if (is_shader_resource) {
		CreateShaderResourceView();
		CreateSampler();
	}
	if (is_render_target) {
		CreateRenderTargetView();
	}
}

void Texture::CreateTexture() {
	D3D11_TEXTURE2D_DESC tex_desc;
	ZeroMemory(&tex_desc, sizeof(tex_desc));
	tex_desc.Width = size[0];
	tex_desc.Height = size[1];
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tex_desc.SampleDesc = { 1, 0 }; // Count, quality
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	if (!is_render_target && is_shader_resource) {
		tex_desc.Usage = D3D11_USAGE_DYNAMIC;
	}
	tex_desc.BindFlags = 0;
	if (is_shader_resource) { tex_desc.BindFlags |= D3D11_BIND_SHADER_RESOURCE; }
	if (is_render_target) { tex_desc.BindFlags |= D3D11_BIND_RENDER_TARGET; }
	tex_desc.CPUAccessFlags = 0;
	if (!is_render_target && is_shader_resource) {
		tex_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	tex_desc.MiscFlags = 0;
	// Currently provides no initial data to the texture
	// The texture that is created is stored in the texture attribute
	device_interface->CreateTexture2D(&tex_desc, NULL, &texture);
	owns_texture = true;
}

void DepthTexture::CreateTexture() {
	D3D11_TEXTURE2D_DESC tex_desc;
	ZeroMemory(&tex_desc, sizeof(tex_desc));
	tex_desc.Width = size[0];
	tex_desc.Height = size[1];
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.SampleDesc = { 1, 0 }; // Count, quality
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	if (!is_render_target && is_shader_resource) {
		tex_desc.Usage = D3D11_USAGE_DYNAMIC;
	}
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;
	device_interface->CreateTexture2D(&tex_desc, NULL, &texture);
}

void Texture::CreateShaderResourceView() {
	D3D11_TEXTURE2D_DESC tex_desc;
	texture->GetDesc(&tex_desc);

	D3D11_SHADER_RESOURCE_VIEW_DESC shad_res_view_desc;
	ZeroMemory(&shad_res_view_desc, sizeof(shad_res_view_desc));
	shad_res_view_desc.Format = tex_desc.Format;
	shad_res_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shad_res_view_desc.Texture2D.MostDetailedMip = 0;
	shad_res_view_desc.Texture2D.MipLevels = 1;

	device_interface->CreateShaderResourceView(texture, &shad_res_view_desc, &shader_resource_view);
}

void Texture::CreateSampler() {
	D3D11_SAMPLER_DESC samp_desc;
	ZeroMemory(&samp_desc, sizeof(samp_desc));
	//samp_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samp_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	samp_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samp_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samp_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samp_desc.MipLODBias = 0;
	samp_desc.MaxAnisotropy = 1; // Shouldn't be needed because not using anisotropy
	samp_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samp_desc.MinLOD = 0;
	samp_desc.MaxLOD = 0;

	device_interface->CreateSamplerState(&samp_desc, &sampler);
}

void Texture::CreateRenderTargetView() {
	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
	ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
	render_target_view_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	render_target_view_desc.Texture2D.MipSlice = 0;

	if (owns_texture) {
		device_interface->CreateRenderTargetView(texture, &render_target_view_desc, &render_target_view);
	}
	else {
		device_interface->CreateRenderTargetView(texture, NULL, &render_target_view);
	}
}

void Texture::SetAsResource(int texture_num, int sampler_num) {
	texture_number = texture_num;
	sampler_number = sampler_num;
	device_context->PSSetShaderResources(texture_number, 1, &shader_resource_view);
	device_context->PSSetSamplers(sampler_number, 1, &sampler);
}

void Texture::UnsetAsResource() {
	void* null_resource = NULL;
	device_context->PSSetShaderResources(texture_number, 1, (ID3D11ShaderResourceView**)&null_resource);
	device_context->PSSetSamplers(sampler_number, 1, (ID3D11SamplerState**)&null_resource);
	texture_number = -1;
	sampler_number = -1;
}

void Texture::Fill() {
	D3D11_MAPPED_SUBRESOURCE ms;
	device_context->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
	byte* data = new byte[ms.RowPitch * 128];
	for (int row = 0; row < 128; row++) {
		float* row_ptr = (float*)(data + ms.RowPitch*row);
		for (int i = 0; i < 128 * 4; i += 4) {
			row_ptr[i + 0] = ((float)i)/(128.0*4);
			row_ptr[i + 1] = 0.0f;
			row_ptr[i + 2] = 1.0f;
			row_ptr[i + 3] = 1.0f;
		}
	}
	memcpy(ms.pData, data, ms.RowPitch*128);
	delete[] data;
	device_context->Unmap(texture, 0);
}

ID3D11Texture2D* Texture::GetTexture() {
	return texture;
}

ID3D11RenderTargetView* Texture::GetRenderTargetView() {
	return render_target_view;
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView() {
	return shader_resource_view;
}