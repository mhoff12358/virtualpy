#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera() {
	location = { { 0.0f, 0.0f, 0.0f } };
	orientaiton = { { 0.0f, 0.0f, 0.0f } };
	vertical_fov = 90.0f/180.0f*3.1415;
}

void PerspectiveCamera::BuildProjectionMatrix() {
	DirectX::XMStoreFloat4x4(&projection_matrix, DirectX::XMMatrixPerspectiveFovRH(
		vertical_fov,
		aspect_ratio,
		0.0000001f,
		10.0f));
}