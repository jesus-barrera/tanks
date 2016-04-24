#include <SDL.h>

#include "../include/globales.h"
#include "../include/utiles.h"
#include "../include/Boton.h"

SDL_Color Boton::color_principal    = {0xFF, 0xFF, 0xFF};
SDL_Color Boton::color_seleccionado = {0xFF, 0x00, 0x00};

Boton::Boton(string texto, int x, int y, int tam_fuente) 
		: Etiqueta(texto, x, y, tam_fuente, Boton::color_principal), 
		  Hoverable(&this->rect) {

	this->seleccionado = false;
}

void Boton::renderizar() {
	bool is_mouse_over = this->isMouseOver();

	if (this->seleccionado != is_mouse_over) {
		// Actualizar color
		if (is_mouse_over) {
			this->fijarColor(Boton::color_seleccionado);
		} else {
			this->fijarColor(Boton::color_principal);
		}

		this->seleccionado = is_mouse_over;
	}

	Etiqueta::renderizar();
}

/**
 * Encuentra el boton seleccionado en un arreglo de botones
 */
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