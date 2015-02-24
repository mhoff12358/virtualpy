#pragma once

#include <Windows.h>

#include <chrono>
#include <array>
#include <vector>
#include <mutex>
#include <atomic>
#include <list>
#include <time.h>

struct PositionState {
	PositionState();
	std::array<float, 3> location;
	std::array<float, 3> scale;
	std::array<float, 4> orientation; // quaternion in the form x, y, z, w
};

struct EntityState {
	int entity_id;
	int display_state; // If 0, don't draw, if 1, draw
	PositionState position;
};

struct FrameState {
	std::array<float, 3> color;
	std::vector<EntityState> entities;

	PositionState camera_position;

	EntityState* GetEntityStateForId(int entity_id);
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
	FrameState InterpolateBetweenFrameStates(FrameState state_0, FrameState state_1, float weight);
	EntityState InterpolateBetweenEntityStates(EntityState state_0, EntityState state_1, float weight);
	PositionState InterpolateBetweenPositionStates(PositionState state_0, PositionState state_1, float weight);

	template<std::size_t SIZE>
	std::array<float, SIZE> InterpolateBetweenArrays(std::array<float, SIZE> array_0, std::array<float, SIZE> array_1, float weight);
	template<std::size_t SIZE>
	std::array<float, SIZE> ScaleBetweenArrays(std::array<float, SIZE> array_0, std::array<float, SIZE> array_1, float array_0_scale, float array_1_scale);

	virtual FrameState InterpolateStateFromBuffer(int num_available_states, int* number_states_unused, long long interpolate_time) = 0;
	
	FrameStateBuffer* frame_state_buffer;
};

class SecondFractionNoPredictInterpolater : public FrameStateInterpolater {
public:
	SecondFractionNoPredictInterpolater(FrameStateBuffer* fsb, float num_seconds);

private:
	virtual FrameState InterpolateStateFromBuffer(int num_available_states, int* number_states_unused, long long interpolate_time);

	// Predicted time between frames. If the latest frame happened at time T,
	// the latest two frames will interpolate to have it be current at
	// T+frame_timing_prediction.
	LARGE_INTEGER frame_timing_prediction;
};

template<std::size_t SIZE>
std::array<float, SIZE> FrameStateInterpolater::InterpolateBetweenArrays(std::array<float, SIZE> array_0, std::array<float, SIZE> array_1, float weight) {
	return ScaleBetweenArrays(array_0, array_1, (1 - weight), weight);
}

template<std::size_t SIZE>
std::array<float, SIZE> FrameStateInterpolater::ScaleBetweenArrays(std::array<float, SIZE> array_0, std::array<float, SIZE> array_1, float array_0_scale, float array_1_scale) {
	std::array<float, SIZE> new_array;
	for (size_t i = 0; i < SIZE; i++) {
		new_array[i] = array_0[i] * array_0_scale + array_1[i] * array_1_scale;
	}
	return new_array;
}
