#ifndef _MAPA_INFO_DAO_H_INCLUDED_
#define _MAPA_INFO_DAO_H_INCLUDED_

#include <fstream>
#include <SDL.h>
#include "tipos.h"

#define DESP_REGISTROS (2 * sizeof(Uint32))

#define MAXLEN_NOMBRE_MAPA 20
#define MAXLEN_RUTA_MAPA 50

#include <string>

using namespace std;

struct MapaInfo {
    Uint32 id;
    char nombre[MAXLEN_NOMBRE_MAPA + 1];
    char ruta[MAXLEN_RUTA_MAPA + 1];
    SDL_bool en_juego;
};

class MapaInfoDAO {
private:
    string nombre_archivo;
    SDL_RWops *archivo;

    int buscarRegistro(Uint32 id);
    void moverPuntero(int index);
    Uint32 obtenerTotalRegistros();

    void abrir();
    void cerrar();

public:
    bool fijarArchivo(const char *nombre_archivo);

    // Carga todos los registros
    Uint32 cargarDatos(MapaInfo **registros);

    // Elimina un registro por id
    bool eliminar(Uint32 id);

    // Recupera un registro por id
    MapaInfo *obtener(Uint32 id, MapaInfo *dst);

    // Guarda un nuevo registro
    MapaInfo *nuevo(const char *nombre, const char *ruta, SDL_bool en_juego = SDL_FALSE);

    // Actualiza los datos de un registro
    bool actualizar(MapaInfo *registro);

    void liberarMem(MapaInfo **registros);
};

#endif