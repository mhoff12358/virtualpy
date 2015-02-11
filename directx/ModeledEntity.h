#pragma once

#include "Entity.h"

#include "Model.h"
#include "ConstantBuffer.h"

class ModeledEntity : public Entity {
public:
	void Initialize(Model* mod, ConstantBuffer* trans, int shad_no);

	virtual void Draw(RenderMode& render_mode);

private:
	// The modeled entity keeps no ownership over the model or the transformation
	Model* model;
	ConstantBuffer* transformation;
	int shader_number;
};
