#include "ModeledEntity.h"

void ModeledEntity::Initialize(Model* mod, ConstantBuffer* trans, int shad_no) {
	model = mod;
	transformation = trans;
	shader_number = shad_no;
}

void ModeledEntity::Draw(RenderMode& render_mode) {
	render_mode.PrepareShader(shader_number);
	transformation->ActivateBuffer(0);
	render_mode.RenderModel(*model);
}