#ifndef _JUGAR_H_INCLUDED_
#define _JUGAR_H_INCLUDED_

#include <SDL.h>

#include "Escena.h"
#include "Tanque.h"
#include "Base.h"
#include "Boton.h"

class Jugar: public Escena {
public: 
	static Tanque *jugador;
	static Tanque *tanque_j1;
	static Tanque *tanque_j2;
	static Base *base_1;
	static Base *base_2;
	static Boton *boton_salir;

	static void inicializar();
	static void liberar();
	
	// Métodos de Escena
	void entrar();
	void actualizar();
	void manejarEvento(SDL_Event &evento);
};

#endif