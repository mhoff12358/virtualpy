#pragma once

#include <Windows.h>

#include <array>
#include <list>
#include <mutex>
#include <atomic>
#include <utility>

namespace virtualpy {

	struct IOState {
		std::array<BYTE, 256> keyboard_at_request;
		std::array<BYTE, 256> keyboard_since_request;

		void ClearSince();
		void UpdateWithNewState(std::array<BYTE, 256> src_state);
	};

	class IOStateBuffer {
	public:
		IOStateBuffer();

		IOState ReadState();
		void WriteState(std::array<BYTE, 256> new_state);

	private:
		std::pair<std::mutex, IOState>* GetPrimaryState();
		std::pair<std::mutex, IOState>* GetSecondaryState();

		std::pair<std::mutex, IOState> state1;
		std::pair<std::mutex, IOState> state2;

		std::atomic<bool> state1primary;
	};

} // virtualpy