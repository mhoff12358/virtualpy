#include "RenderingPipeline.h"

void RenderingPipeline::Initialize(ViewState* vs, World* world, InputHandler* ih, std::string rl) {
	view_state = vs;
	game_world = world;
	input_handler = ih;
	resource_location = rl;
	back_buffer.Initialize(view_state->device_interface, view_state->device_context, view_state->back_buffer_texture, view_state->window_details.screen_size);
}

void RenderingPipeline::InitializeDepthBuffer(std::array<int, 2> depth_buffer_size) {
	depth_buffer.Initialize(view_state->device_interface, view_state->device_context, depth_buffer_size);

	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	depth_stencil_desc.DepthEnable = true;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

	depth_stencil_desc.StencilEnable = true;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;

	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	view_state->device_interface->CreateDepthStencilState(&depth_stencil_desc, &depth_buffer_state);

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
	depth_stencil_view_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Flags = 0;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;

	view_state->device_interface->CreateDepthStencilView(depth_buffer.GetTexture(), NULL, &depth_buffer_view);
}

void ToScreenRenderingPipeline::Initialize(ViewState* vs, World* world, InputHandler* ih, std::string rl) {
	RenderingPipeline::Initialize(vs, world, ih, rl);

	InitializeDepthBuffer(view_state->window_details.screen_size);

	render_to_back_buffer.Initialize(view_state->device_interface, view_state->device_context, back_buffer.GetRenderTargetView(), depth_buffer_view);
	render_to_back_buffer.SetViewport(view_state->window_details.screen_size, { 0.0f, 1.0f });
	//render_to_back_buffer.AddShader(resource_location + "texture_shaders.hlsl", TEXTUREVERTEX::input_element_desc, TEXTUREVERTEX::input_element_desc_size);
	//render_to_back_buffer.AddShader(resource_location + "shaders.hlsl", COLORVERTEX::input_element_desc, COLORVERTEX::input_element_desc_size);

	// Prepare camera and its transformation matrix buffer
	player_camera.vertical_fov = 120.0f / 180.0f*3.1415f;
	player_camera.aspect_ratio = ((float)view_state->window_details.screen_size[0]) / view_state->window_details.screen_size[1];

	player_camera_transformation.Initialize(view_state->device_interface, view_state->device_context);
	player_camera_transformation.CreateBuffer();
}

void ToScreenRenderingPipeline::Render() {
	// Update the camera's position and transformations based on the world state
	
	player_camera.location = game_world->GetPlayerLocation();
	player_camera.orientaiton = game_world->GetPlayerOrientation();
	player_camera.InvalidateAllMatrices();

	XMStoreFloat4x4(&(player_camera_transformation.GetBufferDataRef().transformation),
		player_camera.GetViewProjectionMatrix()
		);
	player_camera_transformation.PushBuffer();

	FrameState frame_state = input_handler->GetFrameState();
	render_to_back_buffer.Clear(D3DXCOLOR(frame_state.color[0], frame_state.color[1], frame_state.color[2], 1.0f));
	view_state->device_context->ClearDepthStencilView(depth_buffer_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//view_state->device_context->OMSetDepthStencilState(depth_buffer_state, 1);
	render_to_back_buffer.Prepare();
	render_to_back_buffer.PrepareConstantBuffer(&player_camera_transformation, 0);
	game_world->Draw(render_to_back_buffer);

	view_state->swap_chain->Present(0, 0);
}

void ToOculusRenderingPipeline::Initialize(ViewState* vs, World* world, InputHandler* ih, std::string rl, Oculus* ocu) {
	RenderingPipeline::Initialize(vs, world, ih, rl);

	oculus = ocu;
	input_handler = ih;


	InitializeDepthBuffer(OculusHelper::ConvertSizeiToArray(oculus->render_target_viewport.Size));
	
	render_texture.Initialize(view_state->device_interface, view_state->device_context, OculusHelper::ConvertSizeiToArray(oculus->render_target_viewport.Size));

	render_to_texture.Initialize(view_state->device_interface, view_state->device_context, render_texture.GetRenderTargetView(), depth_buffer_view);
	//render_to_texture.AddShader(resource_location + "texture_shaders.hlsl", TEXTUREVERTEX::input_element_desc, TEXTUREVERTEX::input_element_desc_size);
	//render_to_texture.AddShader(resource_location + "shaders.hlsl", COLORVERTEX::input_element_desc, COLORVERTEX::input_element_desc_size);

	oculus->ConfigureBackBufferRendering(view_state, &back_buffer);

	oculus->ConfigureEyeTextureInformation(&render_texture);

	player_camera.vertical_fov = 2 * atan(oculus->head_mounted_display->DefaultEyeFov[0].UpTan);
	player_camera.aspect_ratio = oculus->head_mounted_display->DefaultEyeFov[0].LeftTan / oculus->head_mounted_display->DefaultEyeFov[0].UpTan;

	player_camera_transformation.Initialize(view_state->device_interface, view_state->device_context);
	player_camera_transformation.CreateBuffer();
}

void ToOculusRenderingPipeline::Render() {
	FrameState frame_state = input_handler->GetFrameState();
	render_to_texture.Clear(D3DXCOLOR(frame_state.color[0], frame_state.color[1], frame_state.color[2], 1.0f));
	view_state->device_context->ClearDepthStencilView(depth_buffer_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 1);
	for (int i = 0; i < 2; i++) {
		player_camera.location = 
			OculusHelper::ConvertVector3fToArray(
			OculusHelper::ConvertArrayToVector3f(game_world->GetPlayerLocation()) +
			input_handler->GetHeadOffset() + input_handler->GetEyeOffset(i));
		player_camera.orientaiton = game_world->GetPlayerOrientation();
		player_camera.InvalidateAllMatrices();
		XMStoreFloat4x4(&(player_camera_transformation.GetBufferDataRef().transformation),
			player_camera.GetViewProjectionMatrix()
			);
		player_camera_transformation.PushBuffer();

		std::array<int, 2> viewport_position = { oculus->eye_viewports[i].Pos.x, oculus->eye_viewports[i].Pos.y };
		std::array<int, 2> viewport_size = { oculus->eye_viewports[i].Size.w, oculus->eye_viewports[i].Size.h };
		render_to_texture.SetViewport(viewport_position, viewport_size, { 0.0f, 1.0f });
		render_to_texture.Prepare();
		render_to_texture.PrepareConstantBuffer(&player_camera_transformation, 0);
		view_state->device_context->OMSetDepthStencilState(depth_buffer_state, 1);
		game_world->Draw(render_to_texture);
	}

	ovrHmd_EndFrame(oculus->head_mounted_display, oculus->eye_rendering_pose, &oculus->eye_textures[0].Texture);

}