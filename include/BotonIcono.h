#ifndef _BOTON_ICONO_H_INCLUDED_
#define _BOTON_ICONO_H_INCLUDED_

#include <SDL.h>
#include "Hoverable.h"

class BotonIcono: public Hoverable {
private:
    SDL_Texture *icono;
    SDL_Rect rect;
    int estado_actual;
    int estado_previo;

public:
    enum {
        NO_DEFINIDO,
        MOUSE_FUERA,
        PRESIONADO,
        MOUSE_ENCIMA
    };

    BotonIcono(string ruta_textura, int pos_x = 0, int pos_y = 0, float escala = 1);
    int manejarEvento(SDL_Event &evento);
    void renderizar();
};

#endif
