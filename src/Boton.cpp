#include "../include/Boton.h"

Boton::Boton(char *texto, int x, int y): Objeto(x, y), Hoverable(&this->rect) {
	this->textura = renderizarTexto(string(texto), {0xFF, 0xFF, 0xFF});
	
	SDL_QueryTexture(this->textura, NULL, NULL, &(this->rect.w), &(this->rect.h));
}

Boton::~Boton() {
	SDL_DestroyTexture(this->textura);
}

void Boton::renderizar() {
	SDL_Rect dst_rect;

	dst_rect = this->rect;

	if (this->isMouseOver()) {
		dst_rect.x += BTN_HOVER_OFFSET;
	}

	SDL_RenderCopy(renderer_principal, this->textura, NULL, &dst_rect);
}

int Boton::obtenerBotonSeleccionado(Boton *botones[], int num_botones) {
	int boton_seleccionado = -1;

	for (int i = 0; i < num_botones; i++) {
		if (botones[i]->isMouseOver()) {
			boton_seleccionado = i;
			break;
		}
	}

	return boton_seleccionado;
}