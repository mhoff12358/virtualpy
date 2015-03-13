#include "DrawHandler.h"

void ModeledDrawHandler::Initialize(ShaderPipeline* shad, std::vector<std::pair<ConstantBuffer*, int>> consts, Model* mod) {
	shader = shad;
	constant_buffers = consts;
	model = mod;
}

void TexturedDrawHandler::Initialize(ShaderPipeline* shad, std::vector<std::pair<ConstantBuffer*, int>> consts, Model* mod, Texture* tex, int texno, int samno) {
	ModeledDrawHandler::Initialize(shad, consts, mod);
	texture = tex;
	texture_number = texno;
	sampler_number = samno;
}

void ModeledDrawHandler::Draw(RenderMode& render_mode) {
	render_mode.PrepareShader(shader);
	for (std::pair<ConstantBuffer*, int>& constant_buffer_info : constant_buffers) {
		render_mode.PrepareConstantBuffer(constant_buffer_info.first, constant_buffer_info.second);
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

ShaderPipeline* ModeledDrawHandler::GetShader() {
	return shader;
}