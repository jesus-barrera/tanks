#include "../include/Menu.h"

char *etiquetas_botones[NUM_BOTONES] = {
	"Nueva partida",
	"Buscar partida",
	"Editar mapa",
	"Salir"
};

MenuBoton *Menu::botones[NUM_BOTONES];

bool Menu::inicializar() {
	int x, y;

	y = VENTANA_ALTO - MENU_BTN_HEIGHT * NUM_BOTONES;
	x = 15;

	for (int i = 0; i < NUM_BOTONES; i++) {
		botones[i] = new MenuBoton(etiquetas_botones[i], x, y);

		y += MENU_BTN_HEIGHT;
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