#ifndef __ENTITY_H_INCLUDED__
#define __ENTITY_H_INCLUDED__

#include "InputHandler.h"
#include "RenderMode.h"
#include "DrawHandler.h"

class Entity
{
public:
	void Initialize(DrawHandler* dhand);

	virtual void Draw(RenderMode& render_mode);
	virtual void UpdateLogic(const InputHandler& input_handler, int tick_time);

	virtual DrawHandler* GetDrawHandler();

private:
	DrawHandler* draw_handler;
};

#endif
