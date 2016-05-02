#ifndef _JUGAR_H_INCLUDED_
#define _JUGAR_H_INCLUDED_

#include <SDL.h>

#include "Escena.h"
#include "Tanque.h"
#include "Base.h"
#include "Boton.h"

struct MapaInfo;

enum {
	MODO_JUEGO_VIDAS,
	MODO_JUEGO_BASE
};

class Jugar: public Escena {
public:
	// Configuracion del juego
	static int modo_juego;
	static int num_vidas;
	static MapaInfo *mapa_info;
	
	static Tanque *jugador;
	static Tanque *tanque_j1;
	static Tanque *tanque_j2;
	static Base *base_1;
	static Base *base_2;
	static Boton *boton_salir;

	static void inicializar();
	static void liberarMemoria();
	
	// Métodos de Escena
	void entrar();
	void actualizar();
	void manejarEvento(SDL_Event &evento);
};

#endif