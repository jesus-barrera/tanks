#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "Escenario.h"
#include "Boton.h"
#include "utiles.h"
#include "globales.h"

enum {
	BOTON_INICIAR,
	BOTON_CONENCTAR,
	BOTON_EDITAR,
	BOTON_SALIR,
	NUM_BOTONES
};

class Menu {
private:
	static Boton *botones[NUM_BOTONES];

public:
	// Métodos de Escena
	static void actualizar();
	static int manejarEvento(SDL_Event &evento);

	static bool inicializar();
	static void liberarMemoria();
};

#endif // MENU_H