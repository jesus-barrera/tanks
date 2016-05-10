#include <cstring>

#include "../include/Paquete.h"

void Paquete::analizarPqtUnirse(Uint8 *bytes) {
    strcpy(mensaje, (char *)bytes);
    nombre = mensaje;
}

void Paquete::analizarPqtConfiguracion(Uint8 *bytes) {

}

void Paquete::analizarPqtConfirmacion(Uint8 *bytes) {

}

void Paquete::analizarPqtEvento(Uint8 *bytes) {

}

void Paquete::analizarPqtDestruirBloque(Uint8 *bytes) {

}

void Paquete::analizarPqtDestruirObjeto(Uint8 *bytes) {

}

void Paquete::analizarPqtDestruirAbandonar(Uint8 *bytes) {

}

Uint8 *Paquete::escribir(Uint8 *buffer, void *datos, size_t num) {
    memcpy(buffer, datos, num);

    return buffer + num;
}

void Paquete::limpiar() {

}

Paquete::Paquete() {
    limpiar();
}

void Paquete::analizar(Uint8 *bytes) {
    limpiar();

    memcpy(&tipo, bytes, sizeof(tipo));

    bytes += sizeof(tipo);

    if (this->tipo == PQT_UNIRSE) {
        analizarPqtUnirse(bytes);
    }
    // else los otros
}

size_t Paquete::nuevoPqtConfiguracion(Uint8 *buffer, const char *nombre, Uint8 tipo_juego, Sint8 mapa) {
    Uint8 *ptr;
    Uint8 tipo;

    ptr = buffer;
    tipo = PQT_CONFIGURACION;

    ptr = escribir(ptr, &tipo, sizeof(tipo));

    strncpy((char *)ptr, nombre, MAXTAM_NOMBRE_USUARIO);
    ptr[MAXTAM_NOMBRE_USUARIO] = '\0';
    ptr += MAXTAM_NOMBRE_USUARIO + 1;

    ptr = escribir(ptr, &tipo_juego, sizeof(tipo_juego));
    ptr = escribir(ptr, &mapa, sizeof(mapa));

    return (size_t)(ptr - buffer);
}