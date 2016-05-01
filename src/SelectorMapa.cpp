#include <fstream>
#include <iostream>
#include "../include/globales.h"
#include "../include/SelectorMapa.h"
#include "../include/colores.h"

using namespace std;

SelectorMapa::SelectorMapa() {
	mensaje = new Etiqueta("");
	
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

	mensaje->renderizar();
	
	tam = botones.size();
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
void SelectorMapa::cargarMapasInfo() {
	MapaInfo *registros;
	Uint32 num_registros;
	int btn_x, btn_y, btn_sep;

	borrarBotones();

	btn_sep = DEFAULT_FONT_SIZE + 10;
	btn_x = SM_X_OFFSET + 20;
	btn_y = SM_Y_OFFSET + btn_sep + 10;
	
	// Cargar datos del archivo
	registros = mapas_dao.cargarDatos(&num_registros);
	mapas.assign(registros, registros + num_registros); // Copiar a vector
	
	mapas_dao.liberarMem(registros); // liberar memoria reservada

	for (int i = 0; i < num_registros; i++) {
		botones.push_back(new Boton((string)mapas[i].nombre, btn_x, btn_y));
		botones.back()->fijarTamFuente(DEFAULT_FONT_SIZE * 0.85);

		btn_y += btn_sep;
	}

	if (num_registros > 0) {
		mensaje->fijarTexto("Selecciona un mapa");
	} else {
		mensaje->fijarTexto("No se encontraron mapas");
	}
}

/**
 * Agrega un nuevo registro
 */
MapaInfo *SelectorMapa::agregar(string nombre, string ruta) {
	MapaInfo *nuevo;

	nuevo = mapas_dao.nuevo(nombre.c_str(), ruta.c_str());
	mapas.push_back(*nuevo); // Almacenar copia
	cargarMapasInfo(); // TODO: Evitar volver a cargar
	mapas_dao.liberarMem(nuevo);

	return &mapas.back();	 
}