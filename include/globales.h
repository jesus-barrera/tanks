#ifndef _GLOBALES_H_INCLUDED_
#define _GLOBALES_H_INCLUDED_

/*
 * Aqui se definen las constantes y variables globales del juego.
 */ 

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "Escena.h"

#define	TITULO_JUEGO "Tanques"

#define TAMANO_BLOQUE 22
#define MAPA_FILAS    30
#define MAPA_COLUMNAS 36

#define MAPA_ANCHO (MAPA_COLUMNAS * TAMANO_BLOQUE)
#define MAPA_ALTO  (MAPA_FILAS * TAMANO_BLOQUE)

#define PANEL_ANCHO 200
#define VENTANA_ANCHO (MAPA_ANCHO + PANEL_ANCHO)
#define VENTANA_ALTO  MAPA_ALTO
#define VENTANA_PADDING 15

#define PI 3.14593

using namespace std;

extern SDL_Renderer *renderer_principal;
extern SDL_Rect vista_juego;
extern SDL_Rect vista_estatus;
extern Escena *escena;
extern bool salir;

bool registrarEscena(Escena *escena, string nombre);
void irAEscena(string nombre);
void terminarJuego();
void liberarEscenas();

#endif // _GLOBALES_H_INCLUDED_
