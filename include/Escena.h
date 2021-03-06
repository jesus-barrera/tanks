#ifndef _ESCENA_H_INCLUDED_
#define _ESCENA_H_INCLUDED_

#include <SDL.h>

class Escena {
public:
	virtual void entrar() = 0;
	virtual void actualizar() = 0;
    virtual void manejarEvento(SDL_Event &evento) = 0;
	virtual void renderizar() = 0;
};

#endif