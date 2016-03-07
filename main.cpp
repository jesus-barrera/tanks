#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/Tanque.h"
#include "include/Base.h"
#include "include/Escenario.h"
#include "include/Editor.h"
#include "include/Menu.h"
#include "include/tipos.h"
#include "include/utiles.h"

#define	TITULO_JUEGO "Tanques"

using namespace std;

bool inicializar();
void cerrar();
void establecerVistas();
void menu();
void menuManejarEvento(SDL_Event &evento);
void jugar();
void jugarManejarEvento(SDL_Event &evento);
void editar();
void editarManejarEvento(SDL_Event &evento);

SDL_Rect vista_juego;
SDL_Rect vista_estatus;
SDL_Window *ventana_principal;
SDL_Renderer *renderer_principal;
Tanque *tanque_j1, *tanque_j2;

bool salir;
void (*manejarEvento)(SDL_Event &evento);
void (*fnc_actual)(void);

int main(int argc, char* args[]) {
	SDL_Event evento;

	salir = false;
	manejarEvento = &(menuManejarEvento);
	fnc_actual = &menu;

	if (inicializar()) {
		do { 
			while (SDL_PollEvent(&evento)) {
				if (evento.type == SDL_QUIT) {
					salir = true;
				}

				(*manejarEvento)(evento);
			}

			SDL_RenderClear(renderer_principal);

			(*fnc_actual)();

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

	ventana_principal = SDL_CreateWindow(TITULO_JUEGO, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VENTANA_ANCHO, VENTANA_ALTO, SDL_WINDOW_SHOWN);

	if (ventana_principal == NULL) {
		mostrarError("Error al crear ventana");
		return false;
	}

	renderer_principal = SDL_CreateRenderer(ventana_principal, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer_principal == NULL) {
		mostrarError("Error al crear renderer");
		return false;
	}

	SDL_SetRenderDrawColor(renderer_principal, 0x8d, 0x8d, 0x8d, 0xff);
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
	}

	if (!Tanque::inicializar()) {
		mostrarError("Error al inicializar clase Tanque");
		return false;
	} else {
		tanque_j1 = new Tanque();
		tanque_j2 = new Tanque();
	}

	Editor::inicializar();

	return true;

}

void menu() {
	Menu::renderizar();
}

void menuManejarEvento(SDL_Event &evento) {
	int opcion;

	opcion = Menu::manejarEvento(evento);

	switch (opcion) {
		case BOTON_INICIAR:
			Escenario::cargarMapaDesdeArchivo(MAPAS_RUTA"/campo_abierto.map");
			fnc_actual = &jugar;
			manejarEvento = &jugarManejarEvento;
			break;
		case BOTON_EDITAR:
			Escenario::limpiarMapa();
			fnc_actual = &editar;
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
}

void jugarManejarEvento(SDL_Event &evento) {
	tanque_j1->manejarEvento(evento);
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

	Tanque::liberarMemoria();
	Escenario::liberarMemoria();
	Menu::liberarMemoria();

	SDL_DestroyRenderer(renderer_principal);
	renderer_principal = NULL;

	SDL_DestroyWindow(ventana_principal);
	ventana_principal = NULL;

	IMG_Quit();
	SDL_Quit();
}