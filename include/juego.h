#ifndef _JUEGO_H_INCLUDED_
#define _JUEGO_H_INCLUDED_

/*
 * Aqui se definen las constantes y variables globales.
 */ 

#include <SDL.h>
#include <SDL_ttf.h>

#define MAPAS_RUTA "mapas"
#define	TITULO_JUEGO "Tanques"

#define FONT_PTSIZE 28

#define TAMANO_BLOQUE 22
#define MAPA_FILAS    30
#define MAPA_COLUMNAS 36

#define MAPA_ANCHO (MAPA_COLUMNAS * TAMANO_BLOQUE)
#define MAPA_ALTO  (MAPA_FILAS * TAMANO_BLOQUE)

#define VENTANA_ANCHO (MAPA_ANCHO + 125)
#define VENTANA_ALTO  MAPA_ALTO

extern TTF_Font *global_font;
extern SDL_Renderer *renderer_principal;
extern SDL_Rect vista_juego;
extern SDL_Rect vista_estatus;

#endif // _JUEGO_H_INCLUDED_
