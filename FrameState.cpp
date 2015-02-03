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