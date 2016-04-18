#include "../include/Boton.h"

Boton::Boton(char *texto, int x, int y): Objeto(x, y) {
	this->textura = renderizarTexto(string(texto), {0xFF, 0xFF, 0xFF});
	
	SDL_QueryTexture(this->textura, NULL, NULL, &(this->rect.w), &(this->rect.h));
}

Boton::~Boton() {
	SDL_DestroyTexture(this->textura);
}

bool Boton::estaSeleccionado() {
	SDL_Point mouse_pos;

	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

	return SDL_PointInRect(&mouse_pos, &this->rect);
}