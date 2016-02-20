#ifndef _TIPOS_INCLUDE_
#define _TIPOS_INCLUDE_

#include <SDL.h>

#define TAMANO_BLOQUE 20
#define MAPA_FILAS 25	
#define MAPA_COLUMNAS 25

const int MAPA_ANCHO = MAPA_COLUMNAS * TAMANO_BLOQUE;
const int MAPA_ALTO = MAPA_FILAS * TAMANO_BLOQUE;

const int VENTANA_ANCHO = MAPA_ANCHO + 125;
const int VENTANA_ALTO = MAPA_ALTO;

extern SDL_Renderer *renderer_principal;
extern SDL_Rect vista_juego;
extern SDL_Rect vista_estatus;

typedef enum direccion_t {ARRIBA, ABAJO, IZQUIERDA, DERECHA, TOTAL_DIRECCIONES};

#endif // _TIPOS_INCLUDE_