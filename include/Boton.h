#ifndef BOTON_H
#define BOTON_H

#include "Hoverable.h"
#include "Etiqueta.h"

#define BTN_HOVER_OFFSET 20

class Boton: public Etiqueta, public Hoverable {
private:
	static SDL_Color color_principal;
    static SDL_Color color_mouse_over;
	static SDL_Color color_seleccionado;
	bool mouse_is_over;
    bool seleccionado;

public:
	Boton(string texto, int x, int y, int tam_fuente = DEFAULT_FONT_SIZE);
	void renderizar();
    
    bool estaSeleccionado();
    bool estaSeleccionado(bool seleccionado);

	static int obtenerBotonSeleccionado(Boton *botones[], int num_botones);
};

#endif // BOTON_H