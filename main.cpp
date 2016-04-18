#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "include/juego.h"
#include "include/Tanque.h"
#include "include/Base.h"
#include "include/Escenario.h"
#include "include/Editor.h"
#include "include/Menu.h"
#include "include/utiles.h"

using namespace std;

// Inicializa los componentes necesarios
bool inicializar();

// Libera los recursos utilizados por los componentes 
void cerrar();

// Establece las dimensiones de las diferentes vistas del juego
void establecerVistas(); 

// Acciones del menú principal
void menu();
void menuManejarEvento(SDL_Event &evento);

// Acciones durante el juego
void jugar();
void jugarManejarEvento(SDL_Event &evento);

// Acciones del modo edición
void editar();
void editarManejarEvento(SDL_Event &evento);

SDL_Rect vista_juego;
SDL_Rect vista_estatus;
SDL_Window *ventana_principal;
SDL_Renderer *renderer_principal;
TTF_Font *global_font;

Tanque *tanque_j1, *tanque_j2;
Base *base_1, *base_2;

bool salir;
void (*manejarEvento)(SDL_Event &evento);
void (*actualizar)(void);

int main(int argc, char* args[]) {
	SDL_Event evento;

	salir = false;
	manejarEvento = &(menuManejarEvento);
	actualizar = &menu;

	if (inicializar()) {
		do {
			while (SDL_PollEvent(&evento)) {
				if (evento.type == SDL_QUIT) {
					salir = true;
				}

				(*manejarEvento)(evento);
			}

			SDL_SetRenderDrawColor(renderer_principal, 0x8d, 0x8d, 0x8d, 0xff);
			SDL_RenderClear(renderer_principal);

			(*actualizar)();

			SDL_RenderPresent(renderer_principal);
		} while (!salir);
	}

	cerrar();

	return 0;
}

bool inicializar() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		mostrarError("Error al inicializar SDL");
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		mostrarError("Error al inicializar SDL Image");
		return false;
	}

	if (TTF_Init() == -1) {
		mostrarError("Error al inicializar SDL ttf");
		return false;
	} else {
		global_font = TTF_OpenFont("media/fonts/Roboto-Regular.ttf", FONT_PTSIZE);

		if (global_font == NULL) {
			mostrarError("Error al cargar fuente");
			return false;
		}
	}

	ventana_principal = SDL_CreateWindow(
											TITULO_JUEGO, 
											SDL_WINDOWPOS_UNDEFINED, 
											SDL_WINDOWPOS_UNDEFINED, 
											VENTANA_ANCHO, 
											VENTANA_ALTO, 
											SDL_WINDOW_SHOWN
										);

	if (ventana_principal == NULL) {
		mostrarError("Error al crear ventana");
		return false;
	}

	renderer_principal = SDL_CreateRenderer(ventana_principal, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer_principal == NULL) {
		mostrarError("Error al crear renderer");
		return false;
	}

	establecerVistas();

	if (!Escenario::inicializar()) {
		mostrarError("Error al inicializar clase Escenario");
		return false;
	}

	if (!Menu::inicializar()) {
		mostrarError("Error al inicializar clase Menu");
		return false;
	}


	if (!Base::inicializar()) {
		mostrarError("Error al inicializar clase Base");
		return false;
	} else {
		base_1 = new Base();
		base_2 = new Base();
	}

    if (!Bala::inicializar()) {
		mostrarError("Error al inicializar clase bala");
		return false;
	}

	if (!Tanque::inicializar()) {
		mostrarError("Error al inicializar clase Tanque");
		return false;
	} else {
		tanque_j1 = new Tanque();
		tanque_j2 = new Tanque();

		tanque_j1->agregarColisionador(base_1);
		tanque_j1->agregarColisionador(tanque_j2);
		tanque_j1->agregarColisionador(base_2);
	}

	Editor::inicializar(tanque_j1, base_1, tanque_j2, base_2);

	return true;

}

void menu() {
	Escenario::renderizarFondo();
	Menu::renderizar();
}

void menuManejarEvento(SDL_Event &evento) {
	int opcion;

	opcion = Menu::manejarEvento(evento);

	switch (opcion) {
		case BOTON_INICIAR:
			Editor::cargarMapa(MAPAS_RUTA"/campo_abierto.map");
			actualizar = &jugar;
			manejarEvento = &jugarManejarEvento;
			break;
		case BOTON_EDITAR:
			Escenario::limpiarMapa();
			actualizar = &editar;
			manejarEvento = &editarManejarEvento;
			break;
		case BOTON_SALIR:
			salir = true;
			break;
		default:
			;
	};
}

void jugar() {
	tanque_j1->actualizar();
	tanque_j2->actualizar();

	SDL_RenderSetViewport(renderer_principal, &vista_juego);
	Escenario::renderizar();
	tanque_j1->renderizar();
	tanque_j2->renderizar();
	tanque_j1->bala[0].renderizar();
	tanque_j1->bala[1].renderizar();
	tanque_j1->bala[2].renderizar();
	tanque_j2->bala[0].renderizar();
	tanque_j2->bala[1].renderizar();
	tanque_j2->bala[2].renderizar();
	base_1->renderizar();
	base_2->renderizar();
}

void jugarManejarEvento(SDL_Event &evento) {
	tanque_j1->manejarEvento(evento);
	tanque_j2->manejarEvento(evento);
}

void editar() {
	Editor::renderizar();
}

void editarManejarEvento(SDL_Event &evento) {
	Editor::manejarEvento(evento);
}

void establecerVistas() {
	vista_juego.x = 0;
	vista_juego.y = 0;
	vista_juego.w = MAPA_ANCHO;
	vista_juego.h = MAPA_ALTO;

	vista_estatus.x = vista_juego.w;
	vista_estatus.y = 0;
	vista_estatus.w = VENTANA_ANCHO - vista_juego.w;
	vista_estatus.h = VENTANA_ALTO;
}

void cerrar() {
	delete(tanque_j1);
	delete(tanque_j2);
	delete(base_1);
	delete(base_2);

	Tanque::liberarMemoria();
	Escenario::liberarMemoria();
	Menu::liberarMemoria();
	Base::terminar();
	//Bala::liberarMemoria();

	SDL_DestroyRenderer(renderer_principal);
	renderer_principal = NULL;

	SDL_DestroyWindow(ventana_principal);
	ventana_principal = NULL;

	TTF_CloseFont(global_font);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
