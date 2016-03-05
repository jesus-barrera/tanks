#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "Boton.h"
#include "utiles.h"
#include "tipos.h"

enum {
	BOTON_INICIAR,
	BOTON_CONENCTAR,
	BOTON_EDICION,
	BOTON_SALIR,
	NUM_BOTONES
};

class Menu {
private:
	static Boton *botones[NUM_BOTONES];

public:
	static void inicializar();
	static void terminar();
	static void renderizar();
	static int manejarEvento(SDL_Event& evento);
};

#endif // MENU_H