#ifndef _PAQUETE_H_INCLUDED_
#define _PAQUETE_H_INCLUDED_

#include <SDL.h>
#include "globales.h"

enum {
    PQT_UNIRSE,
    PQT_CONFIGURACION,
    PQT_CONFIRMACION,
    PQT_EVENTO,
    PQT_DESTRUIR_BLOQUE,
    PQT_DESTRUIR_OBJETO,
    PQT_ABANDONAR,
    TOTAL_PAQUETES
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
    void limpiar();

public:
    Uint8 tipo;
    
    char mensaje[MAXTAM_NOMBRE_USUARIO + 1];
    char *nombre;
    
    int pos_x;
    int pos_y;

    // Soporte para varios bloques en PQT_DESTRUIR_BLOQUE
    int pos_x2;
    int pos_y2;

    float velocidad;

    Uint8 evento;
    Uint8 num_jugador;
    Uint8 tipo_objeto;

    Paquete();
    void analizar(Uint8 *buffer);
};

#endif
