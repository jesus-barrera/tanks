#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdio>
#include "../include/globales.h"
#include "../include/SelectorMapa.h"
#include "../include/colores.h"

using namespace std;

SelectorMapa::SelectorMapa(int seleccionables) {
	mensaje = new Etiqueta("");

	if (seleccionables & MAPAS_JUEGO){
		et_mapas_juego = new Etiqueta("Mapas del juego", 0, 0, DEFAULT_FONT_SIZE, COLOR_GRIS);
	} else {
		et_mapas_juego = NULL;
	}

	if (seleccionables & MAPAS_USUARIO) {
		et_mapas_usr = new Etiqueta("Mis mapas", 0, 0, DEFAULT_FONT_SIZE, COLOR_GRIS);
	} else {
		et_mapas_usr = NULL;
	}

	mensaje->fijarColor(COLOR_AZUL);
	this->seleccionables = seleccionables;

	indice_seleccionado = -1;
}

SelectorMapa::~SelectorMapa() {
	delete(et_mapas_juego);
	delete(et_mapas_usr);
	delete(mensaje);
	borrarBotones();
}

void SelectorMapa::borrarBotones() {
	for (int i = 0, tam = botones.size(); i < tam; i++) {
		delete(botones[i]);
	}

	indice_seleccionado = -1;
	botones.clear();
}

void SelectorMapa::abrirArchivoPara(MapaInfo *mapa) {
	if (mapa->en_juego) {
		mapas_dao.fijarArchivo(GAME_MAPS_INFO);
	} else {
		mapas_dao.fijarArchivo(USER_MAPS_INFO);
	}
}

int SelectorMapa::buscarPorId(Uint32 id) {
	for (int i = 0, j = mapas.size(); i < j; i++) {
		if (mapas[i].id == id) return i;
	}

	return -1;
}

/**
 * Renderiza un mensaje de ayuda y los botones para seleccionar mapa (si hay)
 */
void SelectorMapa::renderizar() {
	int i, tam;

	mensaje->renderizar();

	if (et_mapas_juego) et_mapas_juego->renderizar();
	if (et_mapas_usr) et_mapas_usr->renderizar();

	for (i = 0, tam = botones.size(); i < tam; i++) {
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
		if (indice_seleccionado != index) {
			if (indice_seleccionado != -1) botones[indice_seleccionado]->estaSeleccionado(false);
			botones[index]->estaSeleccionado(true);
			indice_seleccionado = index;
		}

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
	int btn_x, btn_y, btn_sep, i;

	mapas.clear();
	borrarBotones();

	btn_sep = DEFAULT_FONT_SIZE + 10;
	btn_x = pos_x + 20;
	btn_y = pos_y + btn_sep;

	i = 0;

	if (seleccionables & MAPAS_JUEGO) {
		mapas_dao.fijarArchivo(GAME_MAPS_INFO);
		num_registros = mapas_dao.cargarDatos(&registros);
		mapas.insert(mapas.end(), registros, registros + num_registros);
		mapas_dao.liberarMem(&registros);

		// Posicionar botones
		et_mapas_juego->fijarPosicion(pos_x, btn_y);
		btn_y += btn_sep;

		for (; i < mapas.size(); i++) {
			botones.push_back(new Boton((string)mapas[i].nombre, btn_x, btn_y));
			btn_y += btn_sep;
		}
	}

	if (seleccionables & MAPAS_USUARIO) {
		mapas_dao.fijarArchivo(USER_MAPS_INFO);
		num_registros = mapas_dao.cargarDatos(&registros);
		mapas.insert(mapas.end(), registros, registros + num_registros);
		mapas_dao.liberarMem(&registros);

		// Posicionar botones
		et_mapas_usr->fijarPosicion(pos_x, btn_y);
		btn_y += btn_sep;

		for (; i < mapas.size(); i++) {
			botones.push_back(new Boton((string)mapas[i].nombre, btn_x, btn_y));
			btn_y += btn_sep;
		}
	}

	if (mapas.size() > 0) {
		mensaje->fijarTexto("Selecciona un mapa");
	} else {
		mensaje->fijarTexto("No se encontraron mapas");
	}
}

void SelectorMapa::fijarPosicion(int x, int y) {
	pos_x = x;
	pos_y = y;

	mensaje->fijarPosicion(pos_x, pos_y);
}

/**
 * Agrega un nuevo registro
 */
MapaInfo *SelectorMapa::agregar(string nombre, string ruta, SDL_bool en_juego) {
	MapaInfo *nuevo;
	int i;

	if (en_juego) {
		mapas_dao.fijarArchivo(GAME_MAPS_INFO);
	} else {
		mapas_dao.fijarArchivo(USER_MAPS_INFO);
	}

	nuevo = mapas_dao.nuevo(nombre.c_str(), ruta.c_str(), en_juego);
	cargarMapasInfo(); // TODO: Evitar volver a cargar

	i = buscarPorId(nuevo->id);

	mapas_dao.liberarMem(&nuevo);

	botones[i]->estaSeleccionado(true);
	indice_seleccionado = i;

	return &mapas[i];
}

/**
 * Actualiza el nombre del mapa seleccionado.
 */
bool SelectorMapa::actualizarNombreMapa(string nombre) {
	MapaInfo *mapa;

	mapa = &mapas[indice_seleccionado];

	abrirArchivoPara(mapa);

	strcpy(mapa->nombre, nombre.c_str());

	if (mapas_dao.actualizar(mapa)) {
		botones[indice_seleccionado]->fijarTexto(nombre);

		return true;
	} else {
		return false;
	}
}

/**
 * Elimina el mapa seleccionado.
 */
bool SelectorMapa::eliminarMapa() {
	MapaInfo *mapa;

	mapa = &mapas[indice_seleccionado];

	abrirArchivoPara(mapa);

	if (mapas_dao.eliminar(mapa->id)) {
		remove(mapa->ruta);

		mapas.erase(mapas.begin() + indice_seleccionado);
		botones.erase(botones.begin() + indice_seleccionado);

		indice_seleccionado = -1;

		return true;
	}

	return false;
}
