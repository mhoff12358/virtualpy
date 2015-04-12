#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include <string>

#include "ViewState.h"
#include "RenderMode.h"
#include "World.h"
#include "Oculus.h"

class RenderingPipeline {
public:
	RenderingPipeline() : back_buffer(false, true) {}

	virtual void Initialize(ViewState* vs, World* world, InputHandler* ih, std::string rl);

	virtual void Render() = 0;

	void InitializeDepthBuffer(std::array<int, 2> depth_buffer_size);

	// Should be moved to protected eventually

protected:
	Texture back_buffer;
	DepthTexture depth_buffer;
	ID3D11DepthStencilState* depth_buffer_state;
	ID3D11DepthStencilView* depth_buffer_view;
	ViewState* view_state;
	InputHandler* input_handler;
	World* game_world;
	std::string resource_location;
};

class ToScreenRenderingPipeline : public RenderingPipeline {
public:
	virtual void Initialize(ViewState* vs, World* world, InputHandler* ih, std::string rl);

	void Render();

private:
	RenderMode render_to_back_buffer;
	PerspectiveCamera player_camera;
	ConstantBufferTyped<TransformationMatrixData> player_camera_transformation;
};

class ToOculusRenderingPipeline : public RenderingPipeline {
public:
	ToOculusRenderingPipeline() : render_texture(true, true) {}

	virtual void Initialize(ViewState* vs, World* world, InputHandler* ih, std::string rl, Oculus* ocu);

	void Render();

public:
	Texture render_texture;
	RenderMode render_to_texture;

	Oculus* oculus;

	PerspectiveCamera player_camera;
	ConstantBufferTyped<TransformationMatrixData> player_camera_transformation;
};