#ifndef EDITOR_H
#define EDITOR_H

#include <SDL.h>
#include <fstream>
#include "Escenario.h"
#include "Tanque.h"
#include "Base.h"
#include "tipos.h"
#include "juego.h"

#define PINCEL_MAX 2
#define PINCEL_MIN 1	

using namespace std;

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
	static bool inicializar(Tanque *jugador_1, Base *base_1, Tanque *jugador_2, Base *base_2);
	static void manejarEvento(SDL_Event &evento);
	static void renderizar();
	static void dibujar(int bloque);
	static void insertarObjeto();

	static void cargarMapa();
	static void cargarMapa(const char *nombre_archivo);
	static void guardarMapa();
	static void cargarObjetoInfo(ifstream &intput, Objeto *objeto);
	static void guardarObjetoInfo(ofstream &output, Objeto *objeto);
};

#endif // EDITOR_H