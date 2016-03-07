#include "../include/Boton.h"

Boton::Boton(char *textura_nombre, int x, int y): Objeto(x, y) {
	this->textura = cargarTextura(textura_nombre);
}

Boton::~Boton() {
	SDL_DestroyTexture(this->textura);
}

bool Boton::estaSeleccionado() {
	SDL_Point mouse_pos;

	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

	return SDL_PointInRect(&mouse_pos, &this->rect);
}