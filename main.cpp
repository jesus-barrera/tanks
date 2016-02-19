#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <stdio.h>

#include "include/Tanque.h"
#include "include/tipos.h"

using namespace std;

bool inicializar();
void cerrar();
bool cargarTexturaSuelo();
void renderizarSuelo();
void establecerVistas();
void mostrarError(string message);
void renderizarTodo();
bool manejarTecla(SDL_Keycode key);

SDL_Rect vista_juego;
SDL_Rect vista_estatus;
SDL_Window *ventana_principal;
SDL_Renderer *renderer_principal;
SDL_Texture *textura_suelo;
Tanque *tanque, *t2;

int main(int argc, char* args[]) {
	bool salir;
	SDL_Event evento;
	SDL_Keycode keycode = -1;

	if (inicializar()) {
		do { 
			while (SDL_PollEvent(&evento)) {
				if (evento.type == SDL_QUIT) {
					salir = true;
				}

				tanque->manejarEvento(evento);
			}

			tanque->actualizar();
			renderizarTodo();
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
				
				success = cargarTexturaSuelo() && Tanque::cargarMedios();

				tanque = new Tanque();
			}

		}
	}
	
	return success;
}

bool cargarTexturaSuelo() {
	SDL_Surface* loaded_surface;
	bool success = true;

	loaded_surface = IMG_Load("media/textures/ground_1.png");

	if (loaded_surface == NULL) {
		mostrarError("Error al cargar textura");
		success = false;
	} else {
		textura_suelo = SDL_CreateTextureFromSurface(renderer_principal, loaded_surface);

		if (textura_suelo == NULL) {
			mostrarError("Error al crear textura");
			success = false;
		}

		SDL_FreeSurface(loaded_surface);
	}	

	return success;
}

void renderizarSuelo() {
	int i, j, x_repeat, y_repeat;
	SDL_Rect rect;

	SDL_QueryTexture(textura_suelo, NULL, NULL, &rect.w, &rect.h);

	x_repeat = VENTANA_ANCHO / rect.w;
	y_repeat = VENTANA_ALTO / rect.h;

	for (i = 0; i < x_repeat; i++) {
		rect.x = i * rect.w;

		for (j = 0; j < y_repeat; j++) {
			rect.y = j * rect.h;
			
			SDL_RenderCopy(renderer_principal, textura_suelo, NULL, &rect);
		}
	}
}

void establecerVistas() {
	vista_juego.x = 0;
	vista_juego.y = 0;
	vista_juego.w = VENTANA_ANCHO * 0.8;
	vista_juego.h = VENTANA_ALTO;

	vista_estatus.x = VENTANA_ANCHO * 0.8;
	vista_estatus.y = 0;
	vista_estatus.w = VENTANA_ANCHO * 0.2;
	vista_estatus.h = VENTANA_ALTO;
}

void renderizarTodo() {
	SDL_RenderClear(renderer_principal);
	SDL_RenderSetViewport(renderer_principal, &vista_juego);
	renderizarSuelo();
	tanque->renderizar();
	SDL_RenderPresent(renderer_principal);
}

void cerrar() {
	delete(tanque);
	Tanque::liberarMemoria();

	SDL_DestroyTexture(textura_suelo);
	textura_suelo = NULL;

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