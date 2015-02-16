#pragma once

#include <vector>
#include <utility>

#include "ConstantBuffer.h"
#include "RenderMode.h"
#include "Texture.h"

class DrawHandler {
public:
	virtual void Draw(RenderMode& render_mode) = 0;
};

class ModeledDrawHandler : public DrawHandler {
public:
	void Initialize(int* shano, std::vector<std::pair<ConstantBuffer*, int>> consts, Model* mod);

	virtual void Draw(RenderMode& render_mode) override;
protected:
	int* shader_number;
	std::vector<std::pair<ConstantBuffer*, int>> constant_buffers;
	Model* model;
};

class TexturedDrawHandler : public ModeledDrawHandler {
public:
	void Initialize(int* shano, std::vector<std::pair<ConstantBuffer*, int>> consts, Model* mod, Texture* tex, int texno, int samno);

	virtual void Draw(RenderMode& render_mode) override;

private:
	Texture* texture;
	int texture_number;
	int sampler_number;
};