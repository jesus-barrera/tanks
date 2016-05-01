#ifndef _MAPA_INFO_DAO_H_INCLUDED_
#define _MAPA_INFO_DAO_H_INCLUDED_

#include <fstream>
#include <SDL.h>
#include "tipos.h"

#define ARCH_MAPAS_INFO "mapas/mapas_info.dat"
#define DESP_REGISTROS (2 * sizeof(Uint32))

#define MAXLEN_NOMBRE_MAPA 20
#define MAXLEN_RUTA_MAPA 50

struct MapaInfo {
    Uint32 id;
    char nombre[MAXLEN_NOMBRE_MAPA + 1];
    char ruta[MAXLEN_RUTA_MAPA + 1];
    SDL_bool en_juego;
};

class MapaInfoDAO {
private:
    SDL_RWops *archivo;

    int buscarRegistro(Uint32 id);
    void moverPuntero(int index);
    Uint32 obtenerTotalRegistros();

public:
    MapaInfoDAO();
    ~MapaInfoDAO();

    void abrir();
    void cerrar();

    // Carga todos los registros
    MapaInfo *cargarDatos(Uint32 *num_registros);

    // Elimina un registro por id
    bool eliminar(Uint32 id);

    // Recupera un registro por id
    MapaInfo *obtener(Uint32 id);

    // Guarda un nuevo registro
    MapaInfo *nuevo(const char *nombre, const char *ruta, SDL_bool en_juego = SDL_FALSE);

    // Actualiza los datos de un registro
    bool actualizar(MapaInfo *registro);

    void liberarMem(MapaInfo *registros);
};

#endif