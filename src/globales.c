#include <map>
#include "../include/globales.h"

TTF_Font *global_font;
SDL_Renderer *renderer_principal;
SDL_Rect vista_juego;
SDL_Rect vista_estatus;
Escena *escena;
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

void terminarJuego() {
	salir = true;
}

void liberarEscenas() {
	map<string, Escena *>::iterator it;

	for (it = escenas.begin(); it != escenas.end(); it++) {
		delete(it->second);
	} 

	escenas.clear();
}