#include "IOState.h"

IOStateBuffer::IOStateBuffer() {
	state1primary = true;
}

IOState IOStateBuffer::ReadState() {
	// Gets the primary state as of the time the GetPrimaryState function checks
	auto read_state = GetPrimaryState();
	// If the writer grabs the primary state this will block until it is released,
	// if it is free grabs the lock forcing the writer to swap the primaries.
	read_state->first.lock();
	IOState latest_state_copy = read_state->second;
	read_state->first.unlock();
	return latest_state_copy;
}

void IOStateBuffer::WriteState(IOState new_state) {
	auto primary_state = GetPrimaryState();
	// Attempts to grab the primary state
	if (primary_state->first.try_lock()) {
		// Got the primary state locked, can freely write to it as the reader will block
		primary_state->second = new_state;
		primary_state->first.unlock();
	} else {
		// The primary state was locked, will now grab the secondary and swap
		// the two states. It is fine to determine the secondary state
		// non-atomically with the primary state as this thread is the only one that
		// should be swapping the two.
		auto secondary_state = GetSecondaryState();
		secondary_state->second = new_state;
		state1primary = false;
	}
}

std::pair<std::mutex, IOState>* IOStateBuffer::GetPrimaryState() {
	if (state1primary) {
		return &state1;
	}
	return &state2;
}

std::pair<std::mutex, IOState>* IOStateBuffer::GetSecondaryState() {
	if (state1primary) {
		return &state2;
	}
	return &state1;
}

