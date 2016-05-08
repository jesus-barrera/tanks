#include "../include/utiles.h"
#include "../include/Editor.h"
#include "../include/Escenario.h"
#include "../include/Menu.h"

string etiquetas_botones[NUM_BOTONES] = {
	"Nueva partida",
	"Buscar partida",
	"Crear mapa",
	"Salir"
};

Boton *Menu::botones[NUM_BOTONES];

void Menu::entrar() {
	Editor::cargarMapa(MAPAS_RUTA"campo_abierto.map", NULL, NULL, NULL, NULL);
}

void Menu::actualizar() { }

void Menu::renderizar() {
	SDL_RenderSetViewport(renderer_principal, &vista_juego);

	// Renderizar fondo y bloques
	Escenario::renderizar();

	renderizarCapaGris();
	
	// Renderizar botones
	for (int i = 0; i < NUM_BOTONES; i++) {
		botones[i]->renderizar();
	}
}

void Menu::manejarEvento(SDL_Event &evento) {
	if (evento.type == SDL_MOUSEBUTTONDOWN) {
		switch (Boton::obtenerBotonSeleccionado(Menu::botones, NUM_BOTONES)) {
			case BOTON_INICIAR:
				irAEscena("nueva-partida");
				break;
			case BOTON_EDITAR:
				irAEscena("editar");
				break;
			case BOTON_SALIR:
				terminarJuego();
				break;
			default:
				;
		}
	}
}

bool Menu::inicializar() {
	int btn_x, btn_y;

	btn_y = VENTANA_ALTO - 50 * NUM_BOTONES;
	btn_x = 15;

	for (int i = 0; i < NUM_BOTONES; i++) {
		botones[i] = new Boton(etiquetas_botones[i], btn_x, btn_y, MENU_BTN_TAM);

		btn_y += 50;
	}

	return true;
}

void Menu::liberarMemoria() {
	for (int i = 0; i < NUM_BOTONES; i++) {
		delete(botones[i]);
	}
}