#pragma once

#include <Windows.h>

#include <chrono>

#include "FrameState.h"

class MainLoop
{
public:
	MainLoop(FrameStateInterpolater* ss);
	~MainLoop();

	virtual void Begin() = 0;

protected:
	FrameStateInterpolater* state_source;
	double ticks_per_second;
};

class PrintColor : public MainLoop {
public:
	PrintColor(FrameStateInterpolater* ss);

	void Begin();
};

class WindowsWindow : public MainLoop {
public:
	void Begin();
};