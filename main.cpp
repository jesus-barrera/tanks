#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/Tanque.h"
#include "include/Escenario.h"
#include "include/Menu.h"
#include "include/tipos.h"

using namespace std;

bool inicializar();
void cerrar();
void establecerVistas();
void mostrarError(string message);
void renderizarTodo();

enum {MENU, JUGAR};

SDL_Rect vista_juego;
SDL_Rect vista_estatus;
SDL_Window *ventana_principal;
SDL_Renderer *renderer_principal;
Tanque *tanque_j1, *tanque_j2;

int main(int argc, char* args[]) {
	bool salir;
	SDL_Event evento;

	int vista = MENU;
	int opcion;

	if (inicializar()) {
		do { 
			switch (vista) {
				case MENU:
					opcion = -1;

					while (SDL_PollEvent(&evento)) {
						if (evento.type == SDL_QUIT) {
							salir = true;
						}

						opcion = Menu::manejarEvento(evento);
					}

					if (opcion == BOTON_INICIAR) {
						vista = JUGAR;
					}
					else if (opcion == BOTON_SALIR) {
						salir = true;
					}

					SDL_RenderClear(renderer_principal);
					Menu::renderizar();
					SDL_RenderPresent(renderer_principal);

					break;
				case JUGAR:
					while (SDL_PollEvent(&evento)) {
						if (evento.type == SDL_QUIT) {
							salir = true;
						}

						tanque_j1->manejarEvento(evento);
					}

					tanque_j1->actualizar();
					tanque_j2->actualizar();

					renderizarTodo();
					break;
			}
		} while (!salir);
	}

	cerrar();	

	return 0;
}

bool inicializar() {
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		mostrarError("Error al inicializar SDL");
		success = false;

	} else if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		mostrarError("Error al inicializar SDL Image");
		success = false;

	} else {
		ventana_principal = SDL_CreateWindow("Tanques", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VENTANA_ANCHO, VENTANA_ALTO, SDL_WINDOW_SHOWN);

		if (ventana_principal == NULL) {
			mostrarError("Error al crear ventana");
			success = false;
		} else  {
			renderer_principal = SDL_CreateRenderer(ventana_principal, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (renderer_principal == NULL) {
				mostrarError("Error al crear renderer");
				success = false;
			} else {
				SDL_SetRenderDrawColor(renderer_principal, 0x8d, 0x8d, 0x8d, 0xff);

				establecerVistas();
				
				success = Escenario::inicializar() && Tanque::cargarMedios();

				Menu::inicializar();

				tanque_j1 = new Tanque(0, 100);
				tanque_j2 = new Tanque(100, 100, ABAJO);

				Escenario::cargarMapaDesdeArchivo("mapas/mapa_1.map");
			}

		}
	}
	
	return success;
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

void renderizarTodo() {
	SDL_RenderClear(renderer_principal);
	SDL_RenderSetViewport(renderer_principal, &vista_juego);
	Escenario::renderizarFondo();
	Escenario::renderizarMapa();
	tanque_j1->renderizar();
	SDL_RenderPresent(renderer_principal);
}

void cerrar() {
	delete(tanque_j1);
	delete(tanque_j2);
	Tanque::liberarMemoria();

	Escenario::liberarMemoria();
	Menu::terminar();

	SDL_DestroyRenderer(renderer_principal);
	renderer_principal = NULL;

	SDL_DestroyWindow(ventana_principal);
	ventana_principal = NULL;

	IMG_Quit();
	SDL_Quit();
}

void mostrarError(string message) {
	cout << message << ": " << SDL_GetError() << endl;
}