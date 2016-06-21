#ifndef OBJETO_H
#define OBJETO_H

#include <SDL.h>
#include "tipos.h"
#include "Temporizador.h"

class Objeto {
protected:
	// Dimensiones y posicion asociadas al objeto
	SDL_Rect rect;

	// Textura usada en el renderizado
	SDL_Texture *textura;

	// Valor discreto para la dirección
	direccion_t direccion;

	float pos_x, pos_y;

	// Velociad del objeto, pixeles/frame
	float velocidad;

	// Angulo de dirección
	double angulo;
	
public:
	Objeto(int x = 0, int y = 0, direccion_t direccion = ARRIBA, float velocidad = 0, SDL_Texture *textura = NULL);

	SDL_Rect obtenerRect();

	SDL_Point obtenerPosicion();

	direccion_t obtenerDireccion();

	float obtenerVelocidad();

	SDL_Texture *obtenerTextura();

	int obtenerAlto();
	
	int obtenerAncho();

	// Mueve el objeto a la posición indicada por x y y
	void fijarPosicion(int x, int y);

	// Establece la dirección del objeto (ARRIBA, ABAJO, IZQUIERDA, DERECHA)
	void fijarDireccion(direccion_t direccion);

	// Fija la velocidad del objeto
	void fijarVelocidad(float velocidad);

	void fijarTextura(SDL_Texture *textura);

	void renderizar(SDL_Rect *clip = NULL);

	void rotar(direccion_t direccion);

	void actualizar();

	void mover();
};

#endif // OBJETO_H