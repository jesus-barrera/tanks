#ifndef _TEMPORIZADOR_H_
#define _TEMPORIZADOR_H_

#include <SDL.h>

class Temporizador {
private:
	unsigned int tiempo_inicio;

public:
	Temporizador();
	void iniciar();
	void detener();
	unsigned int obtenerTiempo();
};

#endif
