#include "../include/MenuBoton.h"

SDL_Rect MenuBoton::clips[BOTON_NUM_ESTADOS];

MenuBoton::MenuBoton(char *texto, int x, int y) : Boton(texto, x, y) { }

void MenuBoton::renderizar() {
	SDL_Rect dst_rect;

	dst_rect = this->rect;

	if (this->estaSeleccionado()) {
		dst_rect.x += BTN_HOVER_OFFSET;
	}

	SDL_RenderCopy(renderer_principal, this->textura, NULL, &dst_rect);
}