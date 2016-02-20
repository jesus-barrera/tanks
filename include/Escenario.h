#ifndef _ESCENARIO_INCLUDE_
#define _ESCENARIO_INCLUDE_

#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include "tipos.h"
#include "utiles.h"

#include <iostream>

using namespace std;

class Escenario {
private: 
	static SDL_Texture *textura_suelo;
	static SDL_Texture *textura_bloque;
	static int mapa[MAPA_FILAS][MAPA_COLUMNAS];

public:
	static bool inicializar();

	static void liberarMemoria();

	static void crearMapaAleatorio();

	static void renderizarFondo();

	static void renderizarMapa();

	static bool enColisionConMapa(SDL_Rect &rect);
};

#endif