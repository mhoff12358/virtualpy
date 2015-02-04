#include "MainLoop.h"


MainLoop::MainLoop(FrameStateInterpolater* ss) : state_source(ss)
{
}


MainLoop::~MainLoop()
{
}

PrintColor::PrintColor(FrameStateInterpolater* ss) : MainLoop(ss) {

}

void PrintColor::Begin() {
	while (true) {
		FrameState current_state = state_source->InterpolateCurrentState();
		std::array<float, 3>& screen_color = current_state.color;
		printf("color: %f, %f, %f\n", screen_color[0], screen_color[1], screen_color[2]);
		Sleep(3000);
	}
}