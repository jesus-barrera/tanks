#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "Escena.h"
#include "Boton.h"

#define MENU_BTN_TAM 30

enum {
	BOTON_INICIAR,
	BOTON_CONENCTAR,
	BOTON_EDITAR,
	BOTON_SALIR,
	NUM_BOTONES
};

class Menu: public Escena {
private:
	static Boton *botones[NUM_BOTONES];

public:
	static bool inicializar();
	static void liberarMemoria();

	// Métodos de Escena
	void entrar();
	void actualizar();
	void renderizar();
	void manejarEvento(SDL_Event &evento);
};

#endif // MENU_H