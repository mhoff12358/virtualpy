#pragma once

#include <Windows.h>

#include <chrono>

#include "FrameState.h"
#include "IOState.h"

namespace virtualpy {

	class MainLoop
	{
	public:
		MainLoop(FrameStateInterpolater* ss, IOStateBuffer* iosb);
		~MainLoop();

		virtual void Begin() = 0;

	protected:
		FrameStateInterpolater* state_source;
		IOStateBuffer* io_state_buffer;
		double ticks_per_second;
	};

	class PrintColor : public MainLoop {
	public:
		PrintColor(FrameStateInterpolater* ss, IOStateBuffer* iosb);

		void Begin();
	};

	class WindowsWindow : public MainLoop {
	public:
		void Begin();
	};

} // virtualpy