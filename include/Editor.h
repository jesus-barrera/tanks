#ifndef EDITOR_H
#define EDITOR_H

#include <fstream>
#include <SDL.h>
#include "Escena.h"
#include "Tanque.h"
#include "Base.h"
#include "Boton.h"
#include "BotonIcono.h"
#include "Etiqueta.h"
#include "SelectorMapa.h"
#include "TextInput.h"


#define MAPAS_RUTA "mapas/"
#define MAPAS_USUARIO_RUTA MAPAS_RUTA"user/"
#define MAPA_TIPO ".map"
#define MAXLEN_NOMBRE_MAPA 20
#define EDITOR_TAM_BTN 20

/**
 * Cuando PUBLICAR_MAPA está definido los mapas creados con el editor serán incluidos
 * en el juego.
 */
#define PUBLICAR_MAPA

// Rango de tamaños de pincel
#define PINCEL_MAX 2
#define PINCEL_MIN 1

using namespace std;

class Editor: public Escena {
private:

	// Botones disponibles
	enum {
		BTN_NUEVO,
		BTN_CARGAR,
		BTN_GUARDAR,
		BTN_BORRAR,
		BTN_ATRAS,
		NUM_BTN
	};

	// Estados
	enum {
		ESTADO_SELEC_MAPA,
		ESTADO_EDITAR,
		ESTADO_LEER
	};

	static string iconos[NUM_BTN];

	SelectorMapa *selector_mapa;
	MapaInfo *mapa_info;
	int estado;

	int bloque_seleccionado;
	int tamano_pincel;
	Objeto *objeto_seleccionado;

	Tanque *jugador_1;
	Tanque *jugador_2;
	Base *base_1;
	Base *base_2;

	TextInput *input_nombre;
	BotonIcono *botones[NUM_BTN];
	Boton *cancelar_btn;
	Boton *aceptar_btn;
	Etiqueta *nombre_mapa;

	void manejarEventoBotones(SDL_Event &evento);

	void editarManejarEvento(SDL_Event &evento);
	void selecMapaManejarEvento(SDL_Event &evento);
	void leerTextoManejarEvento(SDL_Event &evento);

public:
	Editor();
	~Editor();

	// Métodos de Escena
	void entrar();
	void actualizar();
	void renderizar();
	void manejarEvento(SDL_Event &evento);

	void dibujar(int bloque);
	void insertarObjeto();

	// Servicio para cargar mapa
	static void cargarMapa(const char *nombre_archivo, Tanque *t1 = NULL, Base *b1 = NULL, Tanque *t2 = NULL, Base *b2 = NULL);
	static void cargarObjetoInfo(SDL_RWops *archivo, Objeto *objeto);

	void botonGuardarPresionado();
	void crearMapa(string nombre);
	void guardarMapa(const char *nombre_archivo);
	void guardarObjetoInfo(SDL_RWops *archivo, Objeto *objeto);
};

#endif // EDITOR_H
