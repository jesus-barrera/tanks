#include <map>
#include "../include/globales.h"

SDL_Renderer *renderer_principal;
SDL_Rect vista_juego;
SDL_Rect vista_estatus;
Escena *escena;
float time_step;
bool salir;

map<string, Escena *> escenas;

bool registrarEscena(Escena *escena, string nombre) {
	if (!escenas.count(nombre)) {
		escenas[nombre] = escena;
		return true;
	} else {
		return false;
	}
}

void irAEscena(string nombre) {
	if (escenas.count(nombre)) {
		escena = escenas[nombre];
		escena->entrar();
	}
}

Escena *obtenerEscena(string nombre) {
	if (escenas.count(nombre)) {
		return escenas[nombre];
	} else {
		return NULL;
	}
}

void liberarEscenas() {
	map<string, Escena *>::iterator it;

	for (it = escenas.begin(); it != escenas.end(); it++) {
		delete(it->second);
	} 

	escenas.clear();
}

void terminarJuego() {
	salir = true;
}