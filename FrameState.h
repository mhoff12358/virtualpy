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

class FrameStateInterpolater {
public:
	FrameStateInterpolater(FrameStateBuffer* fsb) : frame_state_buffer(fsb) {}

	virtual FrameState InterpolateCurrentState() = 0;

protected:
	// Interpolates from state_0 to state_1 with weight representing how far.
	// A weight of 0 means state_0, a weight of 1 means state_1, and a weight
	// above 1 extrapolates beyond state_1 linearly
	FrameState InterpolateBetween(FrameState state_0, FrameState state_1, float weight);

	FrameStateBuffer* frame_state_buffer;
};

class FiveSecondNoPredictInterpolater : public FrameStateInterpolater {
public:
	FiveSecondNoPredictInterpolater(FrameStateBuffer* fsb) : FrameStateInterpolater(fsb) {}

	virtual FrameState InterpolateCurrentState();
};