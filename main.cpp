#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// Objetos
#include "include/Bala.h"
#include "include/Tanque.h"
#include "include/Base.h"
#include "include/Escenario.h"
#include "include/Musica.h"

// Escenas
#include "include/Editor.h"
#include "include/Jugar.h"
#include "include/Menu.h"
#include "include/ConfigurarPartida.h"
#include "include/Conexion.h"

// Utileria
#include "include/globales.h"
#include "include/utiles.h"
#include "include/Temporizador.h"

using namespace std;

// Inicializa los componentes
bool inicializar();

// Libera los recursos utilizados
void cerrar();

// Establece las dimensiones de los diferentes viewports
void establecerVistas();

SDL_Window *ventana_principal;
Temporizador step_timer;

int main(int argc, char* args[]) {
	SDL_Event evento;

	if (inicializar()) {
		salir = false;
		irAEscena("menu");

		do {
			while (SDL_PollEvent(&evento)) {
				if (evento.type == SDL_QUIT) {
					salir = true;
				} else if (evento.type == SDL_WINDOWEVENT_FOCUS_GAINED) {
					SDL_ShowWindow(ventana_principal);
				} else {
					escena->manejarEvento(evento);
				}
			}

			SDL_SetRenderDrawColor(renderer_principal, 0x8d, 0x8d, 0x8d, 0xff);
			SDL_RenderClear(renderer_principal);

			time_step = step_timer.obtenerTiempo() / 1000.0;

			escena->actualizar();

			step_timer.iniciar();

			escena->renderizar();

			SDL_RenderPresent(renderer_principal);
		} while (!salir);
	}

	cerrar();

	return 0;
}

bool inicializar() {

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
	}
    inicializarMusica();
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
	SDL_SetRenderDrawBlendMode(renderer_principal, SDL_BLENDMODE_BLEND);

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

    if (!Bala::inicializar()) {
		mostrarError("Error al inicializar clase bala");
		return false;
	}

	if (!Tanque::inicializar()) {
		mostrarError("Error al inicializar clase Tanque");
		return false;
	}

	Editor::inicializar();
	Conexion::inicializar();

	// Registrar escenas
	registrarEscena(new Menu(), "menu");
	registrarEscena(new Editor(), "editar");
	registrarEscena(new Jugar(), "jugar");
	registrarEscena(new ConfigurarPartida(), "nueva-partida");
	registrarEscena(new Conexion(), "conectar");

	return true;
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
	Tanque::liberarMemoria();
	Escenario::liberarMemoria();
	Menu::liberarMemoria();
	Editor::liberarMemoria();
	Base::liberarMemoria();
	Conexion::liberarMemoria();
	//Bala::liberarMemoria();

	liberarEscenas();
	cerrarFuentes();

	SDL_DestroyRenderer(renderer_principal);
	renderer_principal = NULL;

	SDL_DestroyWindow(ventana_principal);
	ventana_principal = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
