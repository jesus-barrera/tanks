#include <cstdlib>
#include <cstring>
#include "../include/MapaInfoDAO.h"

void MapaInfoDAO::abrir() {
    if (archivo == NULL) archivo = SDL_RWFromFile(nombre_archivo.c_str(), "r+b");
}

void MapaInfoDAO::cerrar() {
    if (archivo != NULL) SDL_RWclose(archivo);
    archivo = NULL;
}

bool MapaInfoDAO::fijarArchivo(const char *nombre_archivo) {
    bool exito = true;

    this->nombre_archivo.assign(nombre_archivo);

    archivo = SDL_RWFromFile(nombre_archivo, "rb");

    if (archivo == NULL) {
        Uint32 id_count;
        Uint32 num_registros;

        archivo = SDL_RWFromFile(nombre_archivo, "wb");

        if (archivo) {
            // Inicializar
            id_count = 0;
            num_registros = 0;
            SDL_RWwrite(archivo, &id_count, sizeof(Uint32), 1);
            SDL_RWwrite(archivo, &num_registros, sizeof(Uint32), 1);
        } else {
            exito = false;
        }

    }

    cerrar();

    return exito;
}

int MapaInfoDAO::buscarRegistro(Uint32 id) {
    MapaInfo registro;
    Uint32 id_obtenido;
    Uint32 num_registros;

    num_registros = obtenerTotalRegistros();

    for (int i = 0; i < num_registros; i++) {
        moverPuntero(i);
        SDL_RWread(archivo, &id_obtenido, sizeof(Uint32), 1); // Leer id del registro

        if (id == id_obtenido) {
            return i;
        }
    }

    return -1;
}

void MapaInfoDAO::moverPuntero(int index) {
    SDL_RWseek(archivo, sizeof(MapaInfo) * index + DESP_REGISTROS, RW_SEEK_SET);   
}

Uint32 MapaInfoDAO::obtenerTotalRegistros() {
    Uint32 num_registros;

    SDL_RWseek(archivo, sizeof(Uint32), RW_SEEK_SET); // Desplazar campo id_count

    SDL_RWread(archivo, &num_registros, sizeof(Uint32), 1); // Leer num_registros 

    return num_registros;
}

// Carga todos los registros
Uint32 MapaInfoDAO::cargarDatos(MapaInfo **registros) {
    Uint32 num_registros;
    abrir();
    
    *registros = NULL;
    num_registros = obtenerTotalRegistros();

    if (num_registros > 0) {
        *registros = (MapaInfo *)malloc(sizeof(MapaInfo) * (num_registros));
        SDL_RWread(archivo, *registros, sizeof(MapaInfo), num_registros);
    }

    cerrar();
    return num_registros;
}

// Elimina un registro por id
bool MapaInfoDAO::eliminar(Uint32 id) {
    int pos;
    bool exito;
    
    abrir();

    if ((pos = buscarRegistro(id)) >= 0) {
        MapaInfo registro;
        Uint32 num_registros;
        int i;

        num_registros = obtenerTotalRegistros() - 1;
        i = pos;

        while (i < num_registros) {
            moverPuntero(i + 1);
            SDL_RWread(archivo, &registro, sizeof(MapaInfo), 1);
            moverPuntero(i);
            SDL_RWwrite(archivo, &registro, sizeof(MapaInfo), 1);
            i--;
        }

        SDL_RWseek(archivo, sizeof(Uint32), RW_SEEK_SET);
        SDL_RWwrite(archivo, &num_registros, sizeof(Uint32), 1);
        
        exito = true;
    } else {
        exito = false;
    }

    cerrar();
    return exito;
}

// Recupera un registro por id
MapaInfo *MapaInfoDAO::obtener(Uint32 id, MapaInfo *dst) {
    int pos;
    MapaInfo *registro;
    
    abrir();
    
    registro = NULL;

    if ((pos = buscarRegistro(id)) >= 0) {
        moverPuntero(pos);
        SDL_RWread(archivo, dst, sizeof(MapaInfo), 1);

        registro = dst;
    }

    cerrar();
    return registro;
}

// Guarda un nuevo registro
MapaInfo *MapaInfoDAO::nuevo(const char *nombre, const char *ruta, SDL_bool en_juego) {
    Uint32 id_count;
    Uint32 num_registros;
    MapaInfo *registro;

    abrir();
    
    registro = (MapaInfo *)malloc(sizeof(MapaInfo));

    strcpy(registro->nombre, nombre);
    strcpy(registro->ruta, ruta);

    registro->en_juego = en_juego;

    SDL_RWseek(archivo, 0, RW_SEEK_SET);
    SDL_RWread(archivo, &id_count, sizeof(Uint32), 1);
    SDL_RWread(archivo, &num_registros, sizeof(Uint32), 1);

    registro->id = ++id_count;

    moverPuntero(num_registros);
    SDL_RWwrite(archivo, registro, sizeof(MapaInfo), 1);
    num_registros++;

    SDL_RWseek(archivo, 0, RW_SEEK_SET);
    SDL_RWwrite(archivo, &id_count, sizeof(Uint32), 1);
    SDL_RWwrite(archivo, &num_registros, sizeof(Uint32), 1);

    cerrar();
    return registro;
}

// Actualiza los datos de un registro
bool MapaInfoDAO::actualizar(MapaInfo *registro) {
    int pos;
    bool exito;

    abrir();

    if ((pos = buscarRegistro(registro->id)) >= 0) {
        moverPuntero(pos);
        SDL_RWwrite(archivo, registro, sizeof(MapaInfo), 1);
        
        exito = true;
    } else {
        exito = false;
    }

    cerrar();
    return exito;
}

void MapaInfoDAO::liberarMem(MapaInfo **registro) {
    free(*registro);

    *registro = NULL;
}