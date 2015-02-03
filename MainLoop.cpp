#include "MainLoop.h"


MainLoop::MainLoop()
{
	state_buffer = NULL;
}


MainLoop::~MainLoop()
{
}

void MainLoop::Initialize(FrameStateBuffer* sb) {
	state_buffer = sb;
}

void PrintColor::Begin() {
	while (true) {
		int number_of_states = state_buffer->GetNumberOfStates();
		if (number_of_states > 0) {
			std::array<float, 3>& screen_color = state_buffer->GetFirstState()->second.color;
			printf("color: %f, %f, %f\n", screen_color[0], screen_color[1], screen_color[2]);
		}
		Sleep(3000);
	}
}