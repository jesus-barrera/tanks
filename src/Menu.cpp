#include "../include/Menu.h"

char *texturas_botones[NUM_BOTONES] = {
	"media/menu/btn_iniciar.png",
	"media/menu/btn_conectar.png",
	"media/menu/btn_editar.png",
	"media/menu/btn_salir.png"
};

MenuBoton *Menu::botones[NUM_BOTONES];

bool Menu::inicializar() {
	int x, y;

	MenuBoton::inicializar();

	x = (VENTANA_ANCHO / 2) - (MENU_BOTON_ANCHO / 2); // centrar botones
	y = 300;

	for (int i = 0; i < NUM_BOTONES; i++) {
		botones[i] = new MenuBoton(texturas_botones[i], x, y);

		y += (MENU_BOTON_ALTO + 10);
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