#ifndef __WORLD_H_INCLUDED__
#define __WORLD_H_INCLUDED__

#include <array>
#include <map>

#include "PerspectiveCamera.h"
#include "InputHandler.h"
#include "ViewState.h"
#include "Model.h"
#include "ConstantBuffer.h"
#include "RenderMode.h"
#include "Texture.h"
#include "Vertices.h"
#include "DrawHandler.h"
#include "EntityFactory.h"
#include "DXResourcePool.h"

class World {
public:
	World(ViewState* vs);

	void Initialize(InputHandler* ih, DXResourcePool* dxrp);

	void UpdateLogic(int time_delta);

	void Draw(RenderMode& render_mode);

	std::array<float, 3> GetPlayerLocation();
	std::array<float, 4> GetPlayerOrientation();

private:
	ViewState* view_state;
	InputHandler* input_handler;
	DXResourcePool* resource_pool;

	//std::vector<int> entities_to_display;
	std::multimap<int, int> entities_to_display;

	std::array<float, 3> player_location;
	std::array<float, 4> player_orientation;
};

#endif