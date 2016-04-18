#ifndef _HOVERABLE_H_INCLUDED_
#define _HOVERABLE_H_INCLUDED_

#include <SDL.h>

class Hoverable {
private:
	SDL_Rect *area;
	const SDL_Rect *viewport;

public:
	Hoverable(SDL_Rect *area, const SDL_Rect *viewport = NULL);

	void setArea(SDL_Rect *area);
	void setViewport(const SDL_Rect *viewport);

	bool isMouseOver();
};

#endif