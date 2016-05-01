#ifndef _SELECTOR_MAPA_H_
#define _SELECTOR_MAPA_H_

#include <SDL.h>
#include <vector>
#include "Boton.h"
#include "MapaInfoDAO.h"

#define SM_X_OFFSET 15
#define SM_Y_OFFSET 10

class SelectorMapa {
private: 
	MapaInfoDAO mapas_dao;
	vector<MapaInfo> mapas;
	vector<Boton *> botones;
	Etiqueta *mensaje;

public:
	SelectorMapa();
	~SelectorMapa();

	void actualizar();
	MapaInfo *obtenerMapaSelecInfo();
	void borrarBotones();

	void cargarMapasInfo();
	MapaInfo *agregar(string nombre, string ruta);
};

#endif