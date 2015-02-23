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
		for (int i = 0; i < num_states_to_remove; i++) {
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

EntityState* FrameState::GetEntityStateForId(int entity_id) {
	for (EntityState& entity_state : entities) {
		if (entity_state.entity_id == entity_id) {
			return &entity_state;
		}
	}
	return NULL;
}

FrameState FrameStateInterpolater::InterpolateBetweenFrameStates(FrameState state_0, FrameState state_1, float weight) {
	FrameState new_state;
	new_state.color = InterpolateBetweenArrays(state_0.color, state_1.color, weight);
	new_state.camera_position = InterpolateBetweenPositionStates(state_0.camera_position, state_1.camera_position, weight);
	for (EntityState& state_1_entity : state_1.entities) {
		EntityState* state_0_entity = state_0.GetEntityStateForId(state_1_entity.entity_id);
		if (state_0_entity == NULL) {
			new_state.entities.push_back(state_1_entity);
		}
		else {
			new_state.entities.push_back(InterpolateBetweenEntityStates(*state_0_entity, state_1_entity, weight));
		}
	}
	return new_state;
}

EntityState FrameStateInterpolater::InterpolateBetweenEntityStates(EntityState state_0, EntityState state_1, float weight) {
	EntityState new_state;
	new_state.entity_id = state_1.entity_id;
	new_state.display_state = state_1.display_state;
	new_state.position = InterpolateBetweenPositionStates(state_0.position, state_1.position, weight);
	return new_state;
}

PositionState FrameStateInterpolater::InterpolateBetweenPositionStates(PositionState state_0, PositionState state_1, float weight) {
	PositionState new_state;
	new_state.location = InterpolateBetweenArrays(state_0.location, state_1.location, weight);
	new_state.scale = InterpolateBetweenArrays(state_0.scale, state_1.scale, weight);
	float subtended_angle = 0.0f;
	for (int i = 0; i < 4; i++) {
		subtended_angle += state_0.orientation[i] * state_1.orientation[i];
	}
	subtended_angle = acos(subtended_angle);
	if (subtended_angle == 0.0f) {
		new_state.orientation = ScaleBetweenArrays(state_0.orientation, state_1.orientation, 1 - weight, weight);
	}
	else {
		new_state.orientation = ScaleBetweenArrays(state_0.orientation, state_1.orientation, sin((1 - weight)*subtended_angle) / sin(subtended_angle), sin(weight*subtended_angle) / sin(subtended_angle));
	}
	return new_state;
}

FrameState FrameStateInterpolater::InterpolateCurrentState() {
	int number_of_active_states = frame_state_buffer->GetNumberOfStates();
	int number_unused_states = 0;
	LARGE_INTEGER interpolate_time;
	QueryPerformanceCounter(&interpolate_time);
	FrameState interpolated_state = this->InterpolateStateFromBuffer(number_of_active_states, &number_unused_states, (long long)interpolate_time.QuadPart);
	frame_state_buffer->PopStates(number_unused_states);
	return interpolated_state;
}

SecondFractionNoPredictInterpolater::SecondFractionNoPredictInterpolater(FrameStateBuffer* fsb, float num_seconds)
	: FrameStateInterpolater(fsb) {
	QueryPerformanceFrequency(&frame_timing_prediction);
	frame_timing_prediction.QuadPart = frame_timing_prediction.QuadPart * num_seconds;
}

FrameState SecondFractionNoPredictInterpolater::InterpolateStateFromBuffer(int num_available_states, int* number_states_unused, long long interpolate_time) {
	//printf("FSNP: ");
	if (num_available_states >= 2) {
		//printf("n states\n");
		auto first_frame = frame_state_buffer->GetFirstState();
		auto second_frame = first_frame;
		second_frame++;

		for (int i = 0; i < num_available_states - 2; i++) {
			auto third_frame = second_frame;
			third_frame++;
			long long dummy_interpolate_time = third_frame->first.QuadPart;
			long long time_into_frame = dummy_interpolate_time - second_frame->first.QuadPart;
			if (time_into_frame < frame_timing_prediction.QuadPart) {
				second_frame->second = InterpolateBetweenFrameStates(first_frame->second, second_frame->second, ((float)time_into_frame) / frame_timing_prediction.QuadPart);
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
		return InterpolateBetweenFrameStates(first_frame->second, second_frame->second, ((float)time_into_frame) / frame_timing_prediction.QuadPart);
	} else if (num_available_states == 1) {
		//printf("one state\n");
		*number_states_unused = num_available_states - 1;
		return frame_state_buffer->GetFirstState()->second;
	} else {
		//printf("no states\n");
		*number_states_unused = num_available_states - 0;
		return{ { { 0.0f, 0.0f, 0.0f } }, {} };
	}
}