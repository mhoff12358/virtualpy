#include "DrawHandler.h"

void ModeledDrawHandler::Initialize(int* shano, std::vector<std::pair<ConstantBuffer*, int>> consts, Model* mod) {
	shader_number = shano;
	constant_buffers = consts;
	model = mod;
}

void TexturedDrawHandler::Initialize(int* shano, std::vector<std::pair<ConstantBuffer*, int>> consts, Model* mod, Texture* tex, int texno, int samno) {
	ModeledDrawHandler::Initialize(shano, consts, mod);
	texture = tex;
	texture_number = texno;
	sampler_number = samno;
}

void ModeledDrawHandler::Draw(RenderMode& render_mode) {
	render_mode.PrepareShader(*shader_number);
	for (std::pair<ConstantBuffer*, int>& constant_buffer_info : constant_buffers) {
		render_mode.PrepareConstantBuffer(constant_buffer_info.first, constant_buffer_info.second);
		OutputFormatted("MODEL_TRANFORM");
		dumpMatrix(constant_buffer_info.first->GetBufferData().transformation);
	}
	render_mode.RenderModel(*model);
}

void TexturedDrawHandler::Draw(RenderMode& render_mode) {
	texture->SetAsResource(texture_number, sampler_number);
	ModeledDrawHandler::Draw(render_mode);
	texture->UnsetAsResource();
}

std::vector<std::pair<ConstantBuffer*, int>>& ModeledDrawHandler::GetConstantBuffers() {
	return constant_buffers;
}