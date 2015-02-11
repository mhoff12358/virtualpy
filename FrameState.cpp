#include "FrameState.h"

FrameStateBuffer::FrameStateBuffer() {
	num_valid_states = 0;
}

void FrameStateBuffer::PushState(FrameState new_state) {
	write_permission.lock();
	LARGE_INTEGER frame_time;
	QueryPerformanceCounter(&frame_time);
	states.push_back(std::make_pair(frame_time, new_state));
	num_valid_states++;
	write_permission.unlock();
}

void FrameStateBuffer::PopState() {
	states.pop_front();
	num_valid_states--;
}

void FrameStateBuffer::PopStates(int num_states_to_remove) {
	if (num_states_to_remove > 0) {
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
}

int FrameStateBuffer::GetNumberOfStates() {
	return num_valid_states;
}

std::list<std::pair<LARGE_INTEGER, FrameState>>::iterator FrameStateBuffer::GetFirstState() {
	return states.begin();
}

FrameState FrameStateInterpolater::InterpolateBetween(FrameState state_0, FrameState state_1, float weight) {
	FrameState new_state;
	new_state.color[0] = state_1.color[0] * weight + state_0.color[0] * (1 - weight);
	new_state.color[1] = state_1.color[1] * weight + state_0.color[1] * (1 - weight);
	new_state.color[2] = state_1.color[2] * weight + state_0.color[2] * (1 - weight);
	new_state.number_of_entities = state_1.number_of_entities;
	return new_state;
}

FrameState FrameStateInterpolater::InterpolateCurrentState() {
	int number_of_active_states = frame_state_buffer->GetNumberOfStates();
	int number_unused_states = 0;
	LARGE_INTEGER interpolate_time;
	QueryPerformanceCounter(&interpolate_time);
	FrameState interpolated_state = this->InterpolateStateFromBuffer(number_of_active_states, &number_unused_states, (long long)interpolate_time.QuadPart);
	//frame_state_buffer->PopStates(number_unused_states);
	return interpolated_state;
}

FiveSecondNoPredictInterpolater::FiveSecondNoPredictInterpolater(FrameStateBuffer* fsb)
	: FrameStateInterpolater(fsb) {
	QueryPerformanceFrequency(&frame_timing_prediction);
	frame_timing_prediction.QuadPart = frame_timing_prediction.QuadPart * 5;
}

FrameState FiveSecondNoPredictInterpolater::InterpolateStateFromBuffer(int num_available_states, int* number_states_unused, long long interpolate_time) {
	//printf("FSNP: ");
	if (num_available_states >= 2) {
		//printf("n states");
		auto first_frame = frame_state_buffer->GetFirstState();
		auto second_frame = first_frame;
		second_frame++;

		for (int i = 0; i < num_available_states - 2; i++) {
			auto third_frame = second_frame;
			third_frame++;
			long long dummy_interpolate_time = third_frame->first.QuadPart;
			long long time_into_frame = dummy_interpolate_time - second_frame->first.QuadPart;
			if (time_into_frame < frame_timing_prediction.QuadPart) {
				second_frame->second = InterpolateBetween(first_frame->second, second_frame->second, ((float)time_into_frame) / frame_timing_prediction.QuadPart);
			}
			second_frame->first.QuadPart = dummy_interpolate_time - frame_timing_prediction.QuadPart;
			
			first_frame = second_frame;
			second_frame++;
		}
		// Now should have two iterators, first_frame and second_frame.
		// The current time is some time after both
		
		assert(interpolate_time >= first_frame->first.QuadPart);
		assert(interpolate_time >= second_frame->first.QuadPart);
		long long time_into_frame = interpolate_time - second_frame->first.QuadPart;
		if (time_into_frame >= frame_timing_prediction.QuadPart) {
			*number_states_unused = num_available_states - 1;
			return second_frame->second;
		}
		*number_states_unused = num_available_states - 2;
		return InterpolateBetween(first_frame->second, second_frame->second, ((float)time_into_frame) / frame_timing_prediction.QuadPart);
	} else if (num_available_states == 1) {
		//printf("one state\n");
		*number_states_unused = num_available_states - 1;
		return frame_state_buffer->GetFirstState()->second;
	} else {
		//printf("no states\n");
		*number_states_unused = num_available_states - 0;
		return{ { { 0.0f, 0.0f, 0.0f } }, 0 };
	}
}