#pragma once
#include <Windows.h>

#include "MainLoop.h"

class MessageBoxLoop : public MainLoop
{
public:
	MessageBoxLoop(FrameStateInterpolater* ss, IOStateBuffer* iosb);

	void Begin();
};

LRESULT CALLBACK WinMainLoopWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class ColoredWindow : public MainLoop {
public:
	ColoredWindow(FrameStateInterpolater* ss, IOStateBuffer* iosb);

	void Begin();
};