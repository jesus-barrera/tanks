#ifndef EDITOR_H
#define EDITOR_H

#include <fstream>
#include <SDL.h>
#include "Escenario.h"
#include "Escena.h"
#include "Tanque.h"
#include "Base.h"
#include "Boton.h"
#include "tipos.h"
#include "globales.h"

// Rango de tamaños de pincel
#define PINCEL_MAX 2
#define PINCEL_MIN 1	

// Botones disponibles
enum {
	EDITOR_BTN_LIMPIAR,
	EDITOR_BTN_CARGAR,
	EDITOR_BTN_GUARDAR,
	EDITOR_BTN_SALIR,
	EDITOR_NUM_BTN
};

using namespace std;

class Editor: public Escena {
private:
	static int bloque_seleccionado;
	static int tamano_pincel;
	static Tanque *jugador_1;
	static Tanque *jugador_2;
	static Base *base_1;
	static Base *base_2;
	static Objeto *objeto_seleccionado;
	static Boton *botones[EDITOR_NUM_BTN];

public:
	static bool inicializar();
	static void liberarMemoria();
	
	// Métodos de Escena
	void entrar();
	void actualizar();
	void manejarEvento(SDL_Event &evento);

	void dibujar(int bloque);
	void insertarObjeto();

	// Servicio para cargar mapa
	static void cargarMapa(const char *nombre_archivo, Tanque *t1, Base *b1, Tanque *t2, Base *b2);
	static void cargarObjetoInfo(ifstream &intput, Objeto *objeto);
	
	void cargarMapa();
	void guardarMapa();
	void guardarObjetoInfo(ofstream &output, Objeto *objeto);
};

#endif // EDITOR_H