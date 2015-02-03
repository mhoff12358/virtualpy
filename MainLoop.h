#pragma once

#include <Windows.h>

#include "FrameState.h"

class MainLoop
{
public:
	MainLoop();
	~MainLoop();

	void Initialize(FrameStateBuffer* sb);
	virtual void Begin() = 0;

protected:
	FrameStateBuffer* state_buffer;
};

class PrintColor : public MainLoop {
	void Begin();
};

