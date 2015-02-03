#pragma once

#include <array>
#include <mutex>
#include <atomic>
#include <list>
#include <time.h>

struct FrameState {
	std::array<float, 3> color;
};

class FrameStateBuffer {
public:
	FrameStateBuffer();

	void PushState(FrameState new_state);
	void PopState();
	void PopStates(int num_states);

	int GetNumberOfStates();
	std::list<std::pair<clock_t, FrameState>>::iterator GetFirstState();

private:
	std::list<std::pair<clock_t, FrameState>> states;
	std::atomic<int> num_valid_states;
	std::mutex write_permission;
};

