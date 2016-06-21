#ifndef _TEMPORIZADOR_H_
#define _TEMPORIZADOR_H_

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
