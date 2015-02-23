#pragma once

#include <Windows.h>

#include <array>
#include <list>
#include <mutex>
#include <atomic>
#include <utility>

struct IOState {
	std::array<BYTE, 256> keyboard;
};

class IOStateBuffer {
public:
	IOStateBuffer();

	IOState ReadState();
	void WriteState(IOState new_state);

private:
	std::pair<std::mutex, IOState>* GetPrimaryState();
	std::pair<std::mutex, IOState>* GetSecondaryState();

	std::pair<std::mutex, IOState> state1;
	std::pair<std::mutex, IOState> state2;

	std::atomic<bool> state1primary;
};