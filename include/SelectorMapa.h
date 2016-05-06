#ifndef _SELECTOR_MAPA_H_
#define _SELECTOR_MAPA_H_

#include <SDL.h>
#include <vector>
#include "Boton.h"
#include "MapaInfoDAO.h"

class SelectorMapa {
private: 
	MapaInfoDAO mapas_dao;
	vector<MapaInfo> mapas;
	vector<Boton *> botones;
	Etiqueta *mensaje;

	int btn_seleccionado;
	int pos_x;
	int pos_y;

public:
	SelectorMapa();
	~SelectorMapa();

	void actualizar();
	MapaInfo *obtenerMapaSelecInfo();
	void borrarBotones();

	void fijarPosicion(int x, int y);

	void cargarMapasInfo();
	MapaInfo *agregar(string nombre, string ruta);
};

#endif