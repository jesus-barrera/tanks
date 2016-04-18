#include "../include/Menu.h"

char *etiquetas_botones[NUM_BOTONES] = {
	"Nueva partida",
	"Buscar partida",
	"Crear mapa",
	"Salir"
};

Boton *Menu::botones[NUM_BOTONES];

void Menu::actualizar() {
	SDL_RenderSetViewport(renderer_principal, &vista_juego);


	// Renderizar fondo y bloques
	Escenario::renderizar();

	// Dibujar negra transparente
	SDL_SetRenderDrawColor(renderer_principal, 0x00, 0x00, 0x00, 0x99);
	SDL_RenderFillRect(renderer_principal, &vista_juego);
	
	// Renderizar botones
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

bool Menu::inicializar() {
	int btn_x, btn_y;

	btn_y = VENTANA_ALTO - 50 * NUM_BOTONES;
	btn_x = 15;

	for (int i = 0; i < NUM_BOTONES; i++) {
		botones[i] = new Boton(etiquetas_botones[i], btn_x, btn_y);

		btn_y += 50;
	}

	return true;
}

void Menu::liberarMemoria() {
	for (int i = 0; i < NUM_BOTONES; i++) {
		delete(botones[i]);
	}
}