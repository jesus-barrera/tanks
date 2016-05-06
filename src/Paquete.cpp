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