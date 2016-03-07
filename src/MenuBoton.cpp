#include "../include/MenuBoton.h"

SDL_Rect MenuBoton::clips[BOTON_NUM_ESTADOS];

MenuBoton::MenuBoton(char *nombre_textura, int x, int y)
	: Boton(nombre_textura, x, y) {
	this->rect.w = MENU_BOTON_ANCHO;
	this->rect.h = MENU_BOTON_ALTO;
}

void MenuBoton::inicializar() {
	MenuBoton::clips[BOTON_NORMAL].w = 350;
	MenuBoton::clips[BOTON_NORMAL].h = 75;
	MenuBoton::clips[BOTON_NORMAL].x = 0;
	MenuBoton::clips[BOTON_NORMAL].y = 0;

	MenuBoton::clips[BOTON_HOVER].w = 350;
	MenuBoton::clips[BOTON_HOVER].h = 75;
	MenuBoton::clips[BOTON_HOVER].x = 350;
	MenuBoton::clips[BOTON_HOVER].y = 0;
}

void MenuBoton::renderizar() {
	SDL_Rect clip, dst_rect;

	dst_rect = this->rect;

	if (this->estaSeleccionado()) {
		clip = MenuBoton::clips[BOTON_HOVER];
		dst_rect.x += 50;
	} else {
		clip = MenuBoton::clips[BOTON_NORMAL];
	}

	SDL_RenderCopy(renderer_principal, this->textura, &clip, &dst_rect);
}