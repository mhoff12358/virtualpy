#include "WindowsMainLoop.h"


MessageBoxLoop::MessageBoxLoop(FrameStateInterpolater* ss) : MainLoop(ss)
{
}

void MessageBoxLoop::Begin() {
	while (true) {
		FrameState current_state = state_source->InterpolateCurrentState();
		std::array<float, 3> screen_color = current_state.color;
		char message_text[100];
		sprintf_s(message_text, 100, "color: %f, %f, %f\n", screen_color[0], screen_color[1], screen_color[2]);
		
		MessageBox(NULL, message_text, "Yes", MB_OK);
	}
}

ColoredWindow::ColoredWindow(FrameStateInterpolater* ss) : MainLoop(ss) {

}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void ColoredWindow::Begin() {
	// the handle for the window, filled by a function
	HWND hWnd;
	// this struct holds information for the window class
	WNDCLASSEX wc;
	HINSTANCE application_handle = GetModuleHandle(NULL);

	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = application_handle;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	// create the window and use the result as the handle
	hWnd = CreateWindowEx(NULL,
		"WindowClass1",    // name of the window class
		"Our First Windowed Program",   // title of the window
		WS_OVERLAPPEDWINDOW,    // window style
		300,    // x-position of the window
		300,    // y-position of the window
		500,    // width of the window
		400,    // height of the window
		NULL,    // we have no parent window, NULL
		NULL,    // we aren't using menus, NULL
		application_handle,    // application handle
		NULL);    // used with multiple windows, NULL

	// display the window on the screen
	ShowWindow(hWnd, SW_SHOW);

	// enter the main loop:

	// this struct holds Windows event messages
	MSG msg;

	// wait for the next message in the queue, store the result in 'msg'
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// translate keystroke messages into the right format
		TranslateMessage(&msg);

		// send the message to the WindowProc function
		DispatchMessage(&msg);
	}

}

// this is the main message handler for the program
LRESULT CALLBACK WinMainLoopWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort through and find what code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
	case WM_DESTROY:
	{
		// close the application entirely
		PostQuitMessage(0);
		return 0;
	} break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(0, 0, 0)));

		EndPaint(hWnd, &ps);
	} break;
	}

	// Handle any messages the switch statement didn't
	return DefWindowProc(hWnd, message, wParam, lParam);
}