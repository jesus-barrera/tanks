#ifndef EDITOR_H
#define EDITOR_H

#include <fstream>
#include <SDL.h>
#include "Escena.h"
#include "Tanque.h"
#include "Base.h"
#include "Boton.h"
#include "Etiqueta.h"
#include "SelectorMapa.h"
#include "TextInput.h"

#define MAPAS_RUTA "mapas/"
#define MAPAS_USUARIO_RUTA MAPAS_RUTA"user/"
#define ARCH_MAPA_TIPO ".map"
#define MAXLEN_NOMBRE_MAPA 20
#define EDITOR_TAM_BTN 20

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

// Estados
enum {
	EDITOR_ST_SELEC_MAPA,
	EDITOR_ST_EDITAR,
	EDITOR_ST_LEER
};

using namespace std;

class Editor: public Escena {
private:
	static SelectorMapa *selector_mapa;
	static MapaInfo *mapa_info;
	static int estado;

	static int bloque_seleccionado;
	static int tamano_pincel;
	static Objeto *objeto_seleccionado;
	
	static Tanque *jugador_1;
	static Tanque *jugador_2;
	static Base *base_1;
	static Base *base_2;

	static TextInput *input_nombre;
	static Boton *botones[EDITOR_NUM_BTN];
	static Boton *cancelar_btn;
	static Boton *aceptar_btn;
	static Etiqueta *nombre_mapa;

	void editarManejarEvento(SDL_Event &evento);
	void selecMapaManejarEvento(SDL_Event &evento);
	void inputManejarEvento(SDL_Event &evento);
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
	static void cargarObjetoInfo(SDL_RWops *archivo, Objeto *objeto);
	
	void cargarMapa();
	void botonGuardarPresionado();
	void crearMapa(string nombre);
	void guardarMapa(const char *nombre_archivo);
	void guardarObjetoInfo(SDL_RWops *archivo, Objeto *objeto);
};

#endif // EDITOR_H