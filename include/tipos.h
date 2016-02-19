#ifndef _TIPOS_INCLUDE_
#define _TIPOS_INCLUDE_

#include <SDL.h>

#define VENTANA_ANCHO 640
#define VENTANA_ALTO 480

extern SDL_Renderer *renderer_principal;

typedef enum direccion_t {ARRIBA, ABAJO, IZQUIERDA, DERECHA, TOTAL_DIRECCIONES};

#endif // _TIPOS_INCLUDE_