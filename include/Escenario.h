#ifndef _ESCENARIO_INCLUDE_
#define _ESCENARIO_INCLUDE_

#include <vector>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <stdlib.h>
#include <time.h>
#include "tipos.h"
#include "utiles.h"

using namespace std;

enum {
	NO_BLOQUE,
	BLOQUE_BRICK_1,
	BLOQUE_BRICK_2,
	BLOQUE_BRICK_3,
	BLOQUE_METAL,
	BLOQUE_AGUA_1,
	BLOQUE_AGUA_2,
	BLOQUE_ARBOL,
	NUM_BLOQUES
};

typedef struct bloque_pos {
	unsigned int x;
	unsigned int y;
};

class Escenario {
private: 
	static SDL_Texture *textura_suelo;
	static int mapa[MAPA_FILAS][MAPA_COLUMNAS];
	static SDL_Texture *bloques[NUM_BLOQUES];
	static unsigned int agua_anim_tiempo;

public:
	static bool inicializar();

	static void liberarMemoria();

	static void crearMapaAleatorio();

	static void renderizarFondo();

	static void renderizarMapa();

	static bool enColisionConMapa(SDL_Rect &rect);

	static vector<bloque_pos> obtenerBloquesEnColision(SDL_Rect &rect);

	static void destruirBloque(bloque_pos bloque);
};

#endif