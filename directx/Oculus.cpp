#include "Oculus.h"

#include "Windows.h"

void Oculus::Initialize() {
	ovr_Initialize();

	head_mounted_display = ovrHmd_Create(0);

	bool tracking_setup = 
		ovrHmd_ConfigureTracking(head_mounted_display, 
		                         ovrTrackingCap_Orientation |
								 ovrTrackingCap_MagYawCorrection |
								 ovrTrackingCap_Position, 0);
	
	ConfigureEyeViewportInformation();
}

void Oculus::ConfigureEyeViewportInformation() {
	std::array<int, 2> render_target_size;
	eye_fovs[0] = head_mounted_display->DefaultEyeFov[0];
	eye_fovs[1] = head_mounted_display->DefaultEyeFov[1];
	ovrSizei left_eye_texture_size = ovrHmd_GetFovTextureSize(
		head_mounted_display, ovrEye_Left,
		eye_fovs[0], 1.0f);
	ovrSizei right_eye_texture_size = ovrHmd_GetFovTextureSize(
		head_mounted_display, ovrEye_Right,
		eye_fovs[1], 1.0f);
	render_target_size[0] = left_eye_texture_size.w + right_eye_texture_size.w;
	render_target_size[1] = max(left_eye_texture_size.h, right_eye_texture_size.h);
	render_target_viewport = { { 0, 0 }, { render_target_size[0], render_target_size[1] } };
	eye_viewports[0] = { { 0, 0 }, { render_target_size[0] / 2, render_target_size[1] } };
	eye_viewports[1] = { { (render_target_size[0] + 1) / 2, 0 }, { render_target_size[0] / 2, render_target_size[1] } };
}

void Oculus::ConfigureEyeTextureInformation(Texture* render_texture) {
	eye_textures[0].D3D11.Header.API = ovrRenderAPI_D3D11;
	eye_textures[0].D3D11.Header.TextureSize = render_target_viewport.Size;
	eye_textures[0].D3D11.Header.RenderViewport = eye_viewports[0];
	eye_textures[0].D3D11.pTexture = render_texture->GetTexture();
	eye_textures[0].D3D11.pSRView = render_texture->GetShaderResourceView();
	eye_textures[1] = eye_textures[0];
	eye_textures[1].D3D11.Header.TextureSize = render_target_viewport.Size;
	eye_textures[1].D3D11.Header.RenderViewport = eye_viewports[1];
}

void Oculus::ConfigureBackBufferRendering(ViewState* view_state, Texture* back_buffer) {
	ovrD3D11Config oculus_config;
	oculus_config.D3D11.Header.API = ovrRenderAPI_D3D11;
	oculus_config.D3D11.Header.BackBufferSize = OculusHelper::ConvertArrayToSizei(view_state->window_details.screen_size);
	oculus_config.D3D11.Header.Multisample = 1;
	oculus_config.D3D11.pDevice = view_state->device_interface;
	oculus_config.D3D11.pDeviceContext = view_state->device_context;
	oculus_config.D3D11.pBackBufferRT = back_buffer->GetRenderTargetView();
	oculus_config.D3D11.pSwapChain = view_state->swap_chain;

	ovrHmd_ConfigureRendering(head_mounted_display, &oculus_config.Config,
		ovrDistortionCap_Chromatic | ovrDistortionCap_TimeWarp | ovrDistortionCap_Overdrive,
		eye_fovs, eye_rendering_desc);

	ovrHmd_AttachToWindow(head_mounted_display, view_state->window_handler, NULL, NULL);
}

void Oculus::Cleanup() {

	ovrHmd_Destroy(head_mounted_display);

	ovr_Shutdown();
}

std::array<float, 4> OculusHelper::ConvertQuaternionToArray(ovrQuatf in_quaternion) {
	std::array<float, 4> out_quaternion = {in_quaternion.x, in_quaternion.y, in_quaternion.z, in_quaternion.w};
	return out_quaternion;
}

std::array<int, 2> OculusHelper::ConvertSizeiToArray(ovrSizei in_size) {
	std::array<int, 2> out_array = { in_size.w, in_size.h };
	return out_array;
}

ovrSizei OculusHelper::ConvertArrayToSizei(std::array<int, 2> in_size) {
	ovrSizei out_size = { in_size[0], in_size[1] };
	return out_size;
}

std::array<float, 3> OculusHelper::ConvertVector3fToArray(ovrVector3f in_vector) {
	std::array<float, 3> out_array = { in_vector.x, in_vector.y, in_vector.z };
	return out_array;
}

ovrVector3f OculusHelper::ConvertArrayToVector3f(std::array<float, 3> in_array) {
	ovrVector3f out_vector = { in_array[0], in_array[1], in_array[2] };
	return out_vector;
}

ovrVector3f operator+(const ovrVector3f& lhs, const ovrVector3f& rhs) {
	ovrVector3f out_vector = { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
	return out_vector;
}