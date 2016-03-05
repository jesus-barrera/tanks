#include "../include/Menu.h"

#define MENU_BOTON_ANCHO 350
#define MENU_BOTON_ALTO 75

char *texturas_botones[NUM_BOTONES] = {
	"media/menu/btn_iniciar.png",
	"media/menu/btn_conectar.png",
	"media/menu/btn_editar.png",
	"media/menu/btn_salir.png"
};

Boton *Menu::botones[NUM_BOTONES];

bool Menu::inicializar() {
	SDL_Rect boton_rect;
	
	boton_rect.x = (VENTANA_ANCHO / 2) - (MENU_BOTON_ANCHO / 2); // centrar botón
	boton_rect.y = 200;
	boton_rect.w = MENU_BOTON_ANCHO;
	boton_rect.h = MENU_BOTON_ALTO;

	for (int i = 0; i < NUM_BOTONES; i++) {
		botones[i] = new Boton(texturas_botones[i], boton_rect);

		boton_rect.y += (MENU_BOTON_ALTO + 10);
	}

	return true;
}

void Menu::liberarMemoria() {
	for (int i = 0; i < NUM_BOTONES; i++) {
		delete(botones[i]);
	}
}

void Menu::renderizar() {
	for (int i = 0; i < NUM_BOTONES; i++) {
		botones[i]->renderizar();
	}
}

int Menu::manejarEvento(SDL_Event &evento) {
	int boton_seleccionado = -1;

	if (evento.type == SDL_MOUSEBUTTONDOWN) {
		for (int i = 0; i < NUM_BOTONES; i++) {
			if (botones[i]->estaSeleccionado()) {
				boton_seleccionado = i;

				break;
			}
		}
	}

	return boton_seleccionado;
}