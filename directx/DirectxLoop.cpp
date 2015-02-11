#include "DirectxLoop.h"

#pragma comment (lib, "winmm.lib")

DirectxLoop::DirectxLoop(bool uo, std::string rl, FrameStateInterpolater* fsi) : MainLoop(fsi), use_oculus(uo), resource_location(rl) {}

struct VS_CONSTANT_BUFFER{
	DirectX::XMFLOAT4X4 mvp;
};

void DirectxLoop::Begin() {
	ViewState view_state;
	ModelGenerator<TEXTUREVERTEX> texture_rendering_model_gen;
	Model texture_rendering_model;
	PerspectiveCamera player_camera;
	InputHandler input_handler;
	World world(&view_state);
	Oculus oculus;
	RenderingPipeline* render_pipeline;

	// Initialize the oculus resources and inject them
	if (use_oculus) {
		oculus.Initialize();
		input_handler.SetOculusState(&oculus);
		render_pipeline = new ToOculusRenderingPipeline;
	}
	else {
		render_pipeline = new ToScreenRenderingPipeline;
	}

	// Initialize the directx resources and inject them
	input_handler.Initialize(state_source);
	view_state.Initialize(GetModuleHandle(NULL), SW_SHOW);
	if (use_oculus) {
		((ToOculusRenderingPipeline*)render_pipeline)->Initialize(&view_state, &world, &input_handler, resource_location, &oculus);
	}
	else {
		render_pipeline->Initialize(&view_state, &world, &input_handler, resource_location);
	}

	EntityFactory::GetInstance().Initialize(view_state.device_interface, view_state.device_context);

	player_camera.aspect_ratio = ((float)view_state.window_details.screen_size[0]) / view_state.window_details.screen_size[1];
	world.Initialize(&player_camera, &input_handler);

	MSG msg;
	int prev_time = timeGetTime();
	int frame_index = 0;


	player_camera.vertical_fov = 120.0f / 180.0f*3.1415f;
	player_camera.aspect_ratio = ((float)view_state.window_details.screen_size[0]) / view_state.window_details.screen_size[1];


	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}
			else if (msg.message == WM_KEYDOWN) {
			}
		}

		int new_time = timeGetTime();
		int time_delta = new_time - prev_time;
		prev_time = new_time;

		input_handler.UpdateStates(frame_index);
		world.UpdateLogic(input_handler, time_delta);

		render_pipeline->Render();

		++frame_index;
	}

	// clean up DirectX and COM
	view_state.Cleanup();
	if (input_handler.IsOculusActive()) {
		oculus.Cleanup();
	}
}