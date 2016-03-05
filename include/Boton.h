#ifndef BOTON_H
#define BOTON_H

#include <SDL.h>
#include "utiles.h"
#include "tipos.h"

class Boton {
private:
	SDL_Rect rect;
	SDL_Texture *textura;

public:
	Boton(char *textura_nombre, SDL_Rect rect);
	~Boton();
	bool estaSeleccionado();
	void renderizar();
};

#endif // BOTON_H