#ifndef _SELECTOR_MAPA_H_
#define _SELECTOR_MAPA_H_

#include <SDL.h>
#include <vector>
#include "Boton.h"
#include "MapaInfoDAO.h"

class SelectorMapa {
private:
	int seleccionables;
	MapaInfoDAO mapas_dao;
	vector<MapaInfo> mapas;
	vector<Boton *> botones;
	Etiqueta *mensaje;
	Etiqueta *et_mapas_juego;
	Etiqueta *et_mapas_usr;

	int indice_seleccionado;
	int pos_x;
	int pos_y;

	void abrirArchivoPara(MapaInfo *mapa);
	int buscarPorId(Uint32 id);

public:
	static const Uint8 MAPAS_JUEGO     = 0x01;
	static const Uint8 MAPAS_USUARIO   = 0x02;
	static const Uint8 TODOS_LOS_MAPAS = MAPAS_JUEGO | MAPAS_USUARIO;

	SelectorMapa(int seleccionables = TODOS_LOS_MAPAS);
	~SelectorMapa();

	void renderizar();
	MapaInfo *obtenerMapaSelecInfo();
	void borrarBotones();
	void fijarPosicion(int x, int y);
	void cargarMapasInfo();
	MapaInfo *agregar(string nombre, string ruta, SDL_bool en_juego = SDL_FALSE);
	bool actualizarNombreMapa(string nombre);
	bool eliminarMapa();
};

#endif
