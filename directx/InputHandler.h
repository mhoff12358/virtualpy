#ifndef __INPUT_HANDLER_H_INCLUDED__
#define __INPUT_HANDLER_H_INCLUDED__

#include <array>

#include "Windows.h"

#include "Logging.h"
#include "../FrameState.h"
#include "Oculus.h"

class InputHandler {
public:
	InputHandler() : head_pose_center({0.0f, 0.0f, 0.0f}) {}

	void Initialize(FrameStateInterpolater* fsi);

	void UpdateStates(int frame_index);

	bool GetKeyPressed(char key) const;
	FrameState GetFrameState() const;
	ovrPoseStatef GetHeadPose() const;
	ovrVector3f GetHeadOffset() const;
	ovrVector3f GetEyeOffset(int eye_number) const;
	ovrVector3f GetActiveEyeOffset() const;

	void SetActiveEyeNumber(int eye_number);
	void SetEyeOffsets(ovrVector3f left_eye_offset, ovrVector3f right_eye_offset);

	void ResetHeadTracking();
	
	bool IsOculusActive() const;

	void SetOculusState(Oculus* ocu);

	int active_eye_number;
private:
	// State variables
	std::array<BYTE, 256> keyboard_state;
	FrameState active_frame_state;
	FrameStateInterpolater* frame_state_interpolater;

	// Information gathering variables
	Oculus* oculus = NULL;
	ovrVector3f eye_pose_offsets[2];
	ovrVector3f head_pose_center; // The location head offsets are taken from

	static FrameState dud_frame_state;
};

#endif