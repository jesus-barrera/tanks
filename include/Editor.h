#ifndef EDITOR_H
#define EDITOR_H

#include <SDL.h>
#include "Escenario.h"
#include "tipos.h"

class Editor {
private:
	static int bloque_seleccionado;

public:
	static void manejarEvento(SDL_Event &evento);
	static void renderizar();
};

#endif // EDITOR_H