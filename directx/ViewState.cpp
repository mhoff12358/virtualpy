#include "ViewState.h"

LRESULT CALLBACK WindowProc(HWND window_handler, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(window_handler, message, wParam, lParam);
}

void ViewState::Initialize(HINSTANCE ih, int wsc) {
	instance_handler = ih;
	window_show_command = wsc;
	InitializeWindow();
	InitializeD3D(window_details.screen_size);
}

void ViewState::InitializeWindow() {
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance_handler;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, window_details.screen_size[0], window_details.screen_size[1] };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	window_handler = CreateWindowEx(NULL,
		"WindowClass",
		"Our First Direct3D Program",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		instance_handler,
		NULL);

	ShowWindow(window_handler, window_show_command);
}

void ViewState::InitializeD3D(std::array<int, 2> back_buffer_size) {
	// Prepare the swap chain details
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//scd.BufferDesc.Width = window_details.screen_size[0];
	//scd.BufferDesc.Height = window_details.screen_size[1];
	scd.BufferDesc.Width = back_buffer_size[0];
	scd.BufferDesc.Height = back_buffer_size[1];
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = window_handler;
	scd.SampleDesc = { 1, 0 };
	scd.Windowed = TRUE;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Create the device and swap chain for rendering
	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swap_chain,
		&device_interface,
		NULL,
		&device_context);

	swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buffer_texture);
}

void ViewState::Cleanup() {
	CleanupD3D();
}

void ViewState::CleanupD3D() {
	if (swap_chain != NULL) { swap_chain->Release(); swap_chain = NULL; }
	if (device_interface != NULL) { device_interface->Release(); device_interface = NULL; }
	if (device_context != NULL) { device_context->Release(); device_context = NULL; }
}
