#ifndef BOTON_H
#define BOTON_H

#include <SDL.h>
#include "Objeto.h"
#include "Hoverable.h"
#include "utiles.h"

#define BTN_HOVER_OFFSET 20

class Boton: public Objeto, public Hoverable {
public:
	Boton(char *textura_nombre, int x, int y);
	~Boton();
	void renderizar();

	// Encuentra el boton seleccionado en un arreglo de botones
	static int obtenerBotonSeleccionado(Boton *botones[], int num_botones);
};

#endif // BOTON_H