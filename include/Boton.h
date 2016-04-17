#ifndef BOTON_H
#define BOTON_H

#include <SDL.h>
#include "Objeto.h"
#include "utiles.h"

enum {
	BOTON_NORMAL,
	BOTON_HOVER,
	BOTON_NUM_ESTADOS
};

class Boton: public Objeto {
public:
	Boton(char *textura_nombre, int x, int y);
	~Boton();
	bool estaSeleccionado();
};

#endif // BOTON_H