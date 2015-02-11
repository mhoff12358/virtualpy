#ifndef __OCULUS_H_INCLUDED__
#define __OCULUS_H_INCLUDED__

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include "OVR.h"

#include <array>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#include "DirectXMath.h"

#define OVR_D3D_VERSION 11
#include "../Src/OVR_CAPI_D3D.h"

#include "Texture.h"
#include "ViewState.h"

class OculusHelper {
public:
	static std::array<float, 4> ConvertQuaternionToArray(ovrQuatf in_quaternion);

	static std::array<int, 2> ConvertSizeiToArray(ovrSizei in_size);
	static ovrSizei ConvertArrayToSizei(std::array<int, 2> in_size);
	
	static std::array<float, 3> ConvertVector3fToArray(ovrVector3f in_vector);
	static ovrVector3f ConvertArrayToVector3f(std::array<float, 3> in_array);
};

ovrVector3f operator+(const ovrVector3f& lhs, const ovrVector3f& rhs);

class Oculus {
public:
	void Initialize();

	void Cleanup();

	ovrHmd head_mounted_display;

	// Eye display information
	ovrFovPort eye_fovs[2];
	ovrRecti eye_viewports[2];
	ovrRecti render_target_viewport;
	ovrEyeRenderDesc eye_rendering_desc[2];
	ovrD3D11Texture eye_textures[2];

	// Tracking information
	ovrTrackingState tracking_state;
	ovrPosef eye_rendering_pose[2]; // Updated each frame as the head moves

	void ConfigureEyeViewportInformation();
	void ConfigureEyeTextureInformation(Texture* render_texture);
	void ConfigureBackBufferRendering(ViewState* view_state, Texture* back_buffer);
};

#endif