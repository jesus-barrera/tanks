#ifndef _ESCENARIO_INCLUDE_
#define _ESCENARIO_INCLUDE_

#include <vector>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include "Temporizador.h"
#include "tipos.h"
#include "globales.h"
#include "utiles.h"

using namespace std;

enum {
	NO_BLOQUE,
	BLOQUE_BRICK,
	BLOQUE_METAL,
	BLOQUE_AGUA_1,
	BLOQUE_AGUA_2,
	BLOQUE_ARBUSTO,
	NUM_BLOQUES
};

class Escenario {
private: 
	static SDL_Texture *textura_suelo;
	static int mapa[MAPA_FILAS][MAPA_COLUMNAS];
	static SDL_Texture *bloques[NUM_BLOQUES];
	static Temporizador animar_temp;

public:
	static bool inicializar();

	static void liberarMemoria();

	static void renderizarFondo();

	static void renderizarMapa();

	static void renderizar();

	static bool enColisionConMapa(SDL_Rect &rect);

	static vector<SDL_Point> obtenerBloquesEnColision(SDL_Rect &rect);

	static SDL_Point obtenerBloqueEnPunto(int x, int y);

	static void destruirBloque(SDL_Point bloque_pos);

	static void insertarBloque(SDL_Point posicion, int bloque);

	static int obtenerBloque(SDL_Point posicion);

	static void limpiarMapa();

	static SDL_Texture *obtenerTexturaBloque(int bloque);
};

#endif