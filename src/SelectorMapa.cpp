#include <fstream>
#include <iostream>
#include "../include/globales.h"
#include "../include/SelectorMapa.h"
#include "../include/colores.h"

using namespace std;

SelectorMapa::SelectorMapa() {
	mensaje = new Etiqueta("Selecciona un mapa");
	
	mensaje->fijarColor(COLOR_AZUL);
	mensaje->fijarPosicion(SM_X_OFFSET, SM_Y_OFFSET);
}

SelectorMapa::~SelectorMapa() {
	delete(mensaje);
	borrarBotones();
}

void SelectorMapa::borrarBotones() {
	for (int i = 0, tam = botones.size(); i < tam; i++) {
		delete(botones[i]);
	}

	botones.clear();
}

/**
 * Renderiza un mensaje de ayuda y los botones para seleccionar mapa (si hay)
 */
void SelectorMapa::actualizar() {
	int i, tam;

	tam = mapas.size();

	mensaje->renderizar();
	for (i = 0; i < tam; i++) {
		botones[i]->renderizar();
	}
}

/**
 * Retorna la información del mapa seleccionado (mouse encima)
 */
MapaInfo *SelectorMapa::obtenerMapaSelecInfo() {
	int index;

	index = Boton::obtenerBotonSeleccionado(&botones[0], botones.size());

	if (index != -1) {
		return &mapas[index];
	} else {
		return NULL;
	}
}

/**
 * Carga la información de los mapas guardados. Almacena la información en un vector y crea un boton 
 * por cada registro.
 */
int SelectorMapa::cargarMapasInfo() {
	size_t pos;
	int btn_x, btn_y, btn_sep;

	string linea;
	MapaInfo info;
	ifstream archivo(ARCH_MAPAS_INFO);

	// Borrar información previa
	mapas.clear();
	borrarBotones();

	btn_sep = DEFAULT_FONT_SIZE + 10;
	btn_x = SM_X_OFFSET + 20;
	btn_y = SM_Y_OFFSET + btn_sep + 10;

	if (archivo.is_open()) {
		while (getline(archivo, linea)) {
			pos = linea.find(":");

			if (pos != string::npos) {
				info.nombre = linea.substr(0, pos);
				info.ruta   = linea.substr(pos + 1);

				mapas.push_back(info);
				botones.push_back(new Boton(info.nombre, btn_x, btn_y));
				botones.back()->fijarTamFuente(DEFAULT_FONT_SIZE * 0.85);

				btn_y += btn_sep;
			}
		}

		archivo.close();
	}

	if (this->mapas.size() > 0) {
		mensaje->fijarTexto("Selecciona un mapa");
	} else {
		mensaje->fijarTexto("No se encontraron mapas");
	}
}

/**
 * Agrega un nuevo registro
 */
MapaInfo *SelectorMapa::agregar(string nombre, string ruta) {
	MapaInfo info;

	info.nombre = nombre;
	info.ruta = ruta;

	mapas.push_back(info);

	return &mapas.back();	 
}