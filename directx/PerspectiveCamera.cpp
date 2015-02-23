#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera() : Camera() {
	vertical_fov = 90.0f/180.0f*3.1415;
}

void PerspectiveCamera::BuildProjectionMatrix() {
	DirectX::XMStoreFloat4x4(&projection_matrix, DirectX::XMMatrixPerspectiveFovRH(
		vertical_fov,
		aspect_ratio,
		1.0f,
		1000.0f));
}