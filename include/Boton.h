#ifndef BOTON_H
#define BOTON_H

#include "Hoverable.h"
#include "Etiqueta.h"

#define BTN_HOVER_OFFSET 20
#define BTN_DEFAULT_FONT_SIZE 35

class Boton: public Etiqueta, public Hoverable {
private:
	static SDL_Color color_principal;
	static SDL_Color color_seleccionado;
	bool seleccionado;

public:
	Boton(string texto, int x, int y, int tam_fuente = BTN_DEFAULT_FONT_SIZE);
	void renderizar();

	static int obtenerBotonSeleccionado(Boton *botones[], int num_botones);
};

#endif // BOTON_H