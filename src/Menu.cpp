#include "../include/Menu.h"

char *etiquetas_botones[NUM_BOTONES] = {
	"Nueva partida",
	"Buscar partida",
	"Crear mapa",
	"Salir"
};

Boton *Menu::botones[NUM_BOTONES];

bool Menu::inicializar() {
	int btn_x, btn_y;

	btn_y = VENTANA_ALTO - MENU_BTN_HEIGHT * NUM_BOTONES;
	btn_x = 15;

	for (int i = 0; i < NUM_BOTONES; i++) {
		botones[i] = new Boton(etiquetas_botones[i], btn_x, btn_y);

		btn_y += MENU_BTN_HEIGHT;
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
	if (evento.type == SDL_MOUSEBUTTONDOWN) {
		return Boton::obtenerBotonSeleccionado(Menu::botones, NUM_BOTONES);
	} else {
		return -1;
	}
}