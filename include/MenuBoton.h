#ifndef MENU_BOTON_H
#define MENU_BOTON_H

#include <SDL.h>
#include "Boton.h"
#include "utiles.h"


#define BTN_HOVER_OFFSET 20
#define MENU_BTN_HEIGHT (FONT_PTSIZE + 22)

class MenuBoton: public Boton {
private:
	static SDL_Rect clips[BOTON_NUM_ESTADOS];
public:
	MenuBoton(char *nombre_textura, int x, int y);
	void renderizar();
};

#endif // MENU_BOTON_H