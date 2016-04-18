#ifndef _JUEGO_H_INCLUDED_
#define _JUEGO_H_INCLUDED_

#include <SDL.h>

#include "Tanque.h"
#include "Base.h"
#include "globales.h"

class Juego {
public: 
	static Tanque *tanque_j1;
	static Tanque *tanque_j2;
	static Base *base_1;
	static Base *base_2;

	static void entrar();
	static void actualizar();
	static void manejarEvento(SDL_Event &evento);

	static void inicializar();
	static void liberar();
};

#endif