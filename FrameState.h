#pragma once

#include <Windows.h>

#include <chrono>
#include <array>
#include <vector>
#include <mutex>
#include <atomic>
#include <list>
#include <time.h>

struct EntityState {
	int entity_id;
	int display_state; // If 0, don't draw, if 1, draw
	std::array<float, 3> location;
	std::array<float, 4> orientation;
};

struct FrameState {
	std::array<float, 3> color;
	std::vector<EntityState> entities;
};

class FrameStateBuffer {
public:
	FrameStateBuffer();

	void PushState(FrameState new_state);
	void PopState();
	void PopStates(int num_states);

	int GetNumberOfStates();
	std::list<std::pair<LARGE_INTEGER, FrameState>>::iterator GetFirstState();

private:
	std::list<std::pair<LARGE_INTEGER, FrameState>> states;
	std::atomic<int> num_valid_states;
	std::mutex write_permission;
};

class FrameStateInterpolater {
public:
	FrameStateInterpolater(FrameStateBuffer* fsb) : frame_state_buffer(fsb) {}

	FrameState InterpolateCurrentState();

protected:
	// Interpolates from state_0 to state_1 with weight representing how far.
	// A weight of 0 means state_0, a weight of 1 means state_1, and a weight
	// above 1 extrapolates beyond state_1 linearly
	FrameState InterpolateBetween(FrameState state_0, FrameState state_1, float weight);

	virtual FrameState InterpolateStateFromBuffer(int num_available_states, int* number_states_unused, long long interpolate_time) = 0;
	
	FrameStateBuffer* frame_state_buffer;
};

class FiveSecondNoPredictInterpolater : public FrameStateInterpolater {
public:
	FiveSecondNoPredictInterpolater(FrameStateBuffer* fsb);

private:
	virtual FrameState InterpolateStateFromBuffer(int num_available_states, int* number_states_unused, long long interpolate_time);

	// Predicted time between frames. If the latest frame happened at time T,
	// the latest two frames will interpolate to have it be current at
	// T+frame_timing_prediction.
	LARGE_INTEGER frame_timing_prediction;
};