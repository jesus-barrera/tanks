#ifndef _TIPOS_INCLUDE_
#define _TIPOS_INCLUDE_

#include <SDL.h>

#define VENTANA_ANCHO 640
#define VENTANA_ALTO 480
#define TAMANO_BLOQUE 32

extern SDL_Renderer *renderer_principal;
extern SDL_Rect vista_juego;
extern SDL_Rect vista_estatus;

typedef enum direccion_t {ARRIBA, ABAJO, IZQUIERDA, DERECHA, TOTAL_DIRECCIONES};

#endif // _TIPOS_INCLUDE_