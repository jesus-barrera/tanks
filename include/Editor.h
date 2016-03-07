#ifndef EDITOR_H
#define EDITOR_H

#include <SDL.h>
#include "Escenario.h"
#include "Tanque.h"
#include "Base.h"
#include "tipos.h"

#define PINCEL_MAX 2
#define PINCEL_MIN 1	

class Editor {
private:
	static int bloque_seleccionado;
	static int tamano_pincel;
	static Tanque *jugador_1;
	static Tanque *jugador_2;
	static Base *base_1;
	static Base *base_2;
	static Objeto *objeto_seleccionado;

public:
	static bool inicializar();
	static void manejarEvento(SDL_Event &evento);
	static void renderizar();
	static void dibujar(int bloque);
	static void insertarObjeto();
};

#endif // EDITOR_H