#ifndef _ESCENARIO_INCLUDE_
#define _ESCENARIO_INCLUDE_

#include <vector>
#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <time.h>
#include "Temporizador.h"
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

class Escenario {
private: 
	static SDL_Texture *textura_suelo;
	static int mapa[MAPA_FILAS][MAPA_COLUMNAS];
	static SDL_Texture *bloques[NUM_BLOQUES];
	static Temporizador animar_temp;

public:
	static bool inicializar();

	static void liberarMemoria();

	static bool cargarMapaDesdeArchivo(char *nombre_archivo);

	static void renderizarFondo();

	static void renderizarMapa();

	static void renderizar();

	static bool enColisionConMapa(SDL_Rect &rect);

	static vector<SDL_Point> obtenerBloquesEnColision(SDL_Rect &rect);

	static void destruirBloque(SDL_Point bloque_pos);
	
	static void insertarBloque(SDL_Point posicion, int bloque);

	static void cargarMapa();

	static void guardarMapa();

	static void limpiarMapa();

};

#endif