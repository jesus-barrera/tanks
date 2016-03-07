#ifndef MENU_BOTON_H
#define MENU_BOTON_H

#include <SDL.h>
#include "Boton.h"
#include "utiles.h"

#define MENU_BOTON_ANCHO 350
#define MENU_BOTON_ALTO 75

class MenuBoton: public Boton {
private:
	static SDL_Rect clips[BOTON_NUM_ESTADOS];
public:
	MenuBoton(char *nombre_textura, int x, int y);
	static void inicializar();
	void renderizar();
};

#endif // MENU_BOTON_H