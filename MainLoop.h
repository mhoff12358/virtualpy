#pragma once

#include <Windows.h>

#include "FrameState.h"

class MainLoop
{
public:
	MainLoop(FrameStateInterpolater* ss);
	~MainLoop();

	virtual void Begin() = 0;

protected:
	FrameStateInterpolater* state_source;
};

class PrintColor : public MainLoop {
public:
	PrintColor(FrameStateInterpolater* ss);

	void Begin();
};

