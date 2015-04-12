#pragma once

#include <Windows.h>

#include "Quaternion.h"

#include <chrono>
#include <array>
#include <vector>
#include <unordered_map>
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
	EntityState();
	int entity_id;
	int render_bundle_id; // id of the render bundle to draw with. Use -1 to not draw
	PositionState position;
};

struct ConstantBufferState {
	std::array<float, 4> data;
};

struct RenderBundleState {
	RenderBundleState();
	RenderBundleState(int num_const_buff);
	std::vector<ConstantBufferState> constant_buffers;
};

struct FrameState {
	FrameState();
	FrameState(std::array<float, 3> col);
	std::array<float, 3> color;
	std::vector<EntityState> entities;
	std::unordered_map<int, RenderBundleState> render_bundles;

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
	RenderBundleState InterpolateBetweenRenderBundleStates(RenderBundleState state_0, RenderBundleState state_1, float weight);
	ConstantBufferState InterpolateBetweenConstantBufferStates(ConstantBufferState state_0, ConstantBufferState state_1, float weight);
	PositionState InterpolateBetweenPositionStates(PositionState state_0, PositionState state_1, float weight);

	template<std::size_t SIZE>
	std::array<float, SIZE> InterpolateBetweenArrays(std::array<float, SIZE> array_0, std::array<float, SIZE> array_1, float weight);
	template<std::size_t SIZE>
	std::array<float, SIZE> ScaleBetweenArrays(std::array<float, SIZE> array_0, std::array<float, SIZE> array_1, float array_0_scale, float array_1_scale);

	virtual FrameState InterpolateStateFromBuffer(int num_available_states, int* number_states_unused, long long interpolate_time) = 0;
	
	FrameStateBuffer* frame_state_buffer;
};

class ConstantDelayInterpolater : public FrameStateInterpolater {
public:
	ConstantDelayInterpolater(FrameStateBuffer* fsb, float num_seconds);

	// Predicted time between frames. If the latest frame happened at time T,
	// the latest two frames will interpolate to have it be current at
	// T+frame_timing_prediction.
	LARGE_INTEGER frame_timing_prediction;
};

class ConstantDelayPreemptingNoPredictInterpolater : public ConstantDelayInterpolater {
public:
	ConstantDelayPreemptingNoPredictInterpolater(FrameStateBuffer* fsb, float num_seconds) :
		ConstantDelayInterpolater(fsb, num_seconds) {}

private:
	virtual FrameState InterpolateStateFromBuffer(int num_available_states, int* number_states_unused, long long interpolate_time);
};

class ConstantDelayNoPreemptingNoPredictInterpolater : public ConstantDelayInterpolater {
public:
	ConstantDelayNoPreemptingNoPredictInterpolater(FrameStateBuffer* fsb, float num_seconds) :
		ConstantDelayInterpolater(fsb, num_seconds) {}

private:
	virtual FrameState InterpolateStateFromBuffer(int num_available_states, int* number_states_unused, long long interpolate_time);
};

class ConstantDelayNoPreemeptingExtrapolateInterpolater : public ConstantDelayInterpolater {
public:
	ConstantDelayNoPreemeptingExtrapolateInterpolater(FrameStateBuffer* fsb, float num_seconds) :
		ConstantDelayInterpolater(fsb, num_seconds) {}

private:
	virtual FrameState InterpolateStateFromBuffer(int num_available_states, int* number_states_unused, long long interpolate_time);
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
