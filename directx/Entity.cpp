#include "Entity.h"

void Entity::Initialize(DrawHandler* dhand) {
	draw_handler = dhand;
}

void Entity::Draw(RenderMode& render_mode) {
	draw_handler->Draw(render_mode);
}

void Entity::UpdateLogic(const InputHandler& input_handler, int tick_time) {

}