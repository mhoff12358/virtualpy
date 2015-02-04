#include "MainLoop.h"


MainLoop::MainLoop(FrameStateInterpolater* ss) : state_source(ss)
{
	LARGE_INTEGER tps;
	QueryPerformanceFrequency(&tps);
	ticks_per_second = tps.QuadPart;
}


MainLoop::~MainLoop()
{
}

PrintColor::PrintColor(FrameStateInterpolater* ss) : MainLoop(ss) {

}

void PrintColor::Begin() {
	LARGE_INTEGER prev_time, curr_time, time_diff;
	QueryPerformanceCounter(&prev_time);
	
	while (true) {
		QueryPerformanceCounter(&curr_time);
		time_diff.QuadPart = curr_time.QuadPart - prev_time.QuadPart;
		FrameState current_state = state_source->InterpolateCurrentState();
		std::array<float, 3>& screen_color = current_state.color;
		printf("color: %f, %f, %f\t%u\t%f\n", screen_color[0], screen_color[1], screen_color[2], time_diff, CLOCKS_PER_SEC/((float)time_diff.QuadPart));
		prev_time = curr_time;
	}
}