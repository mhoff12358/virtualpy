#include "FrameState.h"

FrameStateBuffer::FrameStateBuffer() {
	num_valid_states = 0;
}

void FrameStateBuffer::PushState(FrameState new_state) {
	write_permission.lock();
	states.push_back(std::make_pair(clock(), new_state));
	num_valid_states++;
	write_permission.unlock();
}

void FrameStateBuffer::PopState() {
	states.pop_front();
	num_valid_states--;
}

void FrameStateBuffer::PopStates(int num_states_to_remove) {
	int current_num_valid_states = num_valid_states;
	if (num_states_to_remove > current_num_valid_states) {
		num_states_to_remove = current_num_valid_states;
	}
	auto first_element_to_remove = states.begin();
	auto last_element_to_remove = states.begin();
	for (int i = 1; i < num_states_to_remove; i++) {
		last_element_to_remove++;
	}
	states.erase(first_element_to_remove, last_element_to_remove);
	num_valid_states.fetch_sub(num_states_to_remove);
}

int FrameStateBuffer::GetNumberOfStates() {
	return num_valid_states;
}

std::list<std::pair<clock_t, FrameState>>::iterator FrameStateBuffer::GetFirstState() {
	return states.begin();
}

FrameState FrameStateInterpolater::InterpolateBetween(FrameState state_0, FrameState state_1, float weight) {
	FrameState new_state;
	new_state.color[0] = state_1.color[0] * weight + state_0.color[0] * (1 - weight);
	new_state.color[1] = state_1.color[1] * weight + state_0.color[1] * (1 - weight);
	new_state.color[2] = state_1.color[2] * weight + state_0.color[2] * (1 - weight);
	return new_state;
}

FrameState FrameStateInterpolater::InterpolateCurrentState() {
	int number_of_active_states = frame_state_buffer->GetNumberOfStates();
	int number_unused_states = 0;
	FrameState interpolated_state = this->InterpolateStateFromBuffer(number_of_active_states, &number_unused_states);
	frame_state_buffer->PopStates(number_unused_states);
	return interpolated_state;
}

FiveSecondNoPredictInterpolater::FiveSecondNoPredictInterpolater(FrameStateBuffer* fsb)
	: FrameStateInterpolater(fsb) {
	frame_timing_prediction = 5 * CLOCKS_PER_SEC;
}

FrameState FiveSecondNoPredictInterpolater::InterpolateStateFromBuffer(int num_available_states, int* number_states_unused) {
	printf("FSNP: ");
	if (num_available_states >= 2) {
		printf("two states\n");
		auto first_state_iter = frame_state_buffer->GetFirstState();
		for (int i = 0; i < num_available_states - 2; i++) {
			first_state_iter++;
		}
		auto second_state_iter = first_state_iter;
		second_state_iter++;
		clock_t interpolate_time = clock();
		assert(interpolate_time >= first_state_iter->first);
		assert(interpolate_time >= second_state_iter->first);
		clock_t time_into_frame = interpolate_time - second_state_iter->first;
		if (time_into_frame >= frame_timing_prediction) {
			*number_states_unused = num_available_states - 1;
			return second_state_iter->second;
		}
		*number_states_unused = num_available_states - 2;
		return InterpolateBetween(first_state_iter->second, second_state_iter->second, ((float)time_into_frame) / frame_timing_prediction);
	} else if (num_available_states == 1) {
		printf("one state\n");
		*number_states_unused = num_available_states - 1;
		return frame_state_buffer->GetFirstState()->second;
	} else {
		printf("no states\n");
		*number_states_unused = num_available_states - 0;
		return{ { { 0.0f, 0.0f, 0.0f } } };
	}
}