#include "../include/Hoverable.h"

Hoverable::Hoverable(SDL_Rect *area, const SDL_Rect *viewport) {
	this->setArea(area);
	this->setViewport(viewport);
}

void Hoverable::setArea(SDL_Rect *area) {
	this->area = area;
}

void Hoverable::setViewport(const SDL_Rect *viewport) {
	this->viewport = viewport;
}


SDL_Rect *Hoverable::getViewport() {
	return (SDL_Rect *)this->viewport;
}

bool Hoverable::isMouseOver() {
	SDL_Point mouse_pos;
	SDL_Rect area = *(this->area);

	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

	if (viewport not_eq NULL) {
		mouse_pos.x -= viewport->x;
		mouse_pos.y -= viewport->y;
	}

	return (mouse_pos.x >= area.x && mouse_pos.x < area.x + area.w) &&
		   (mouse_pos.y >= area.y && mouse_pos.y < area.y + area.h);
}