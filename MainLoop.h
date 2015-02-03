#pragma once

#include <Windows.h>

#include "FrameState.h"

class MainLoop
{
public:
	MainLoop(FrameStateBuffer* sb);
	~MainLoop();

	virtual void Begin() = 0;

protected:
	FrameStateBuffer* state_buffer;
};

class PrintColor : public MainLoop {
public:
	PrintColor(FrameStateBuffer* sb);

	void Begin();
};

