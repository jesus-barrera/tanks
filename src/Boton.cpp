#include "../include/Boton.h"

Boton::Boton(char *textura_nombre, SDL_Rect rect) {
	this->textura = cargarTextura(textura_nombre);

	this->rect = rect;
}

Boton::~Boton() {
	SDL_DestroyTexture(this->textura);
}

bool Boton::estaSeleccionado() {
	SDL_Point mouse_pos;

	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

	return SDL_PointInRect(&mouse_pos, &this->rect);
}

void Boton::renderizar() {
	SDL_Rect clip;
	SDL_Rect rect = this->rect;

	clip.w = rect.w;
	clip.h = rect.h;
	clip.y = 0;


	if (this->estaSeleccionado()) {
		clip.x = clip.w;
		rect.x += 50;
	} else {
		clip.x = 0;
	}	

	SDL_RenderCopy(renderer_principal, this->textura, &clip, &rect);
}