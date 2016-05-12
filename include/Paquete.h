#ifndef _PAQUETE_H_INCLUDED_
#define _PAQUETE_H_INCLUDED_

#include <SDL.h>
#include "globales.h"

#define MAXTAM_MENSAJE 20

enum {
    PQT_UNIRSE,
    PQT_CONFIGURACION,
    PQT_CONFIRMACION,
    PQT_EVENTO,
    PQT_DESTRUIR_BLOQUE,
    PQT_DESTRUIR_OBJETO,
    PQT_ABANDONAR,
    PQT_MANTENER_CONEXION,
    PQT_TERMINAR_PARTIDA,
    TOTAL_PAQUETES
};

enum {
    EVENTO_ARRIBA,
    EVENTO_DERECHA,
    EVENTO_ABAJO,
    EVENTO_IZQUIERDA,
    EVENTO_DISPARO
};

enum {
    TIPO_OBJ_TANQUE,
    TIPO_OBJ_BASE
};

class Paquete {
private:    
    void analizarPqtUnirse(Uint8 *bytes);
    void analizarPqtConfiguracion(Uint8 *bytes);
    void analizarPqtConfirmacion(Uint8 *bytes);
    void analizarPqtEvento(Uint8 *bytes);
    void analizarPqtDestruirBloque(Uint8 *bytes);
    void analizarPqtDestruirObjeto(Uint8 *bytes);
    void analizarPqtDestruirAbandonar(Uint8 *bytes);
    void analizarPqtMantenerConexion(Uint8 *bytes);
    void analizarPqtTerminarPartida(Uint8 *bytes);
    void limpiar();
    
    static Uint8 *escribir(Uint8 *buffer, void *datos, size_t num);
    static Uint8 *leer(Uint8 *src, void *dst, size_t num);

public:
    Uint8 tipo;
    
    char mensaje[MAXTAM_MENSAJE + 1];
    char *nombre;
    
    Sint16 pos_x;
    Sint16 pos_y;

    // Soporte para varios bloques en PQT_DESTRUIR_BLOQUE
    Sint16 pos_x2;
    Sint16 pos_y2;

    float velocidad;

    Uint8 evento;
    Uint8 num_jugador;
    Uint8 bandera;
    Uint8 tipo_objeto;
    Uint8 tipo_juego;
    Sint8 info_mapa;
    Uint8 ganador;

    Paquete();
    void analizar(Uint8 *buffer);

    static size_t nuevoPqtConfiguracion(Uint8 *buffer, const char *nombre, Uint8 tipo_juego, Sint8 mapa);
    static size_t nuevoPqtUnirse(Uint8 *buffer, const char *nombre);
    static size_t nuevoPqtConfirmacion(Uint8 *buffer, const char *msg);
    static size_t nuevoPqtEvento(Uint8 *buffer, Sint16 pos_x, Sint16 pos_y, Uint8 evento, float velocidad);
    static size_t nuevoPqtDestruirBloque(Uint8 *bytes, Sint16 pos_x, Sint16 pos_y, Sint16 pos_x2, Sint16 pos_y2);
    static size_t nuevoPqtDestruirObjeto(Uint8 *bytes, Uint8 num_jugador, Uint8 tipo_objeto);
    static size_t nuevoPqtAbandonar(Uint8 *buffer, const char *msg);
    static size_t nuevoPqtMantenerConexion(Uint8 *buffer, const char *msg);
    static size_t nuevoPqtTerminarPartida(Uint8 *buffer, Uint8 ganador);
};

#endif
