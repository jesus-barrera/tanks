#include <cstring>
#include <arpa/inet.h>

#include "../include/Paquete.h"

void Paquete::analizarPqtUnirse(Uint8 *bytes) {
    strcpy(mensaje, (char *)bytes);
    nombre = mensaje;
}

void Paquete::analizarPqtConfiguracion(Uint8 *bytes) {
    strcpy(mensaje, (char *)bytes);
    nombre = mensaje;
    bytes = bytes + (MAXTAM_MENSAJE + 1);
    memcpy(&tipo_juego, bytes, sizeof(tipo_juego));
    bytes = bytes + sizeof(tipo_juego);

    memcpy(&info_mapa, bytes, sizeof(info_mapa));
}

void Paquete::analizarPqtConfirmacion(Uint8 *bytes) {
    strcpy(mensaje, (char *)bytes);
    nombre = mensaje;
}

void Paquete::analizarPqtEvento(Uint8 *bytes) {
    bytes = leer(bytes, &pos_x, sizeof(pos_x));
    bytes = leer(bytes, &pos_y, sizeof(pos_y));

    pos_x = ntohl(pos_x);
    pos_y = ntohl(pos_y);

    bytes = leer(bytes, &evento, sizeof(evento));
    bytes = leer(bytes, &velocidad, sizeof(velocidad));
}

void Paquete::analizarPqtDestruirBloque(Uint8 *bytes) {
    Uint16 temp1;
    temp1=*bytes;
    pos_x= temp1;
    bytes = bytes + sizeof(temp1);
    temp1 = *bytes;
    pos_y = temp1;
    bytes = bytes + sizeof(temp1);
    temp1 = *bytes;
    pos_x2 = temp1;
    bytes = bytes + sizeof(temp1);
    temp1 = *bytes;
    pos_y2 = temp1;
}

void Paquete::analizarPqtDestruirObjeto(Uint8 *bytes) {
    num_jugador = *bytes;
    bytes = bytes + sizeof(num_jugador);
    tipo_objeto = *bytes;
    bytes = bytes + sizeof(tipo_objeto);
    bandera = *bytes;
}

void Paquete::analizarPqtDestruirAbandonar(Uint8 *bytes) {
    strcpy(mensaje, (char *)bytes);
}

void Paquete::analizarPqtMantenerConexion(Uint8 *bytes) {
    strcpy(mensaje, (char *)bytes);
}

Uint8 *Paquete::escribir(Uint8 *buffer, void *datos, size_t num) {
    memcpy(buffer, datos, num);

    return buffer + num;
}

Uint8 *Paquete::leer(Uint8 *src, void *dst, size_t num) {
    memcpy(dst, src, num);

    return src + num;
}

/*
*Limpiando todas la variables
*/
void Paquete::limpiar() {
    tipo=-1;
    memset(mensaje, 0, sizeof(mensaje));
    nombre=NULL;
    pos_x=0;
    pos_y=0;
    pos_x2=0;
    pos_y2=0;
    velocidad=0;
    evento=0;
    num_jugador=0;
    tipo_objeto=0;
    tipo_juego=0;
    info_mapa=0;
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
    }else{
        if(this->tipo == PQT_CONFIGURACION){
            analizarPqtConfiguracion(bytes);
        }else{
            if(this->tipo == PQT_CONFIRMACION){
                analizarPqtConfirmacion(bytes);
            }else{
                if(this->tipo == PQT_EVENTO){
                    analizarPqtEvento(bytes);
                }else{
                    if(this->tipo == PQT_DESTRUIR_BLOQUE){
                        analizarPqtDestruirBloque(bytes);
                    }else{
                        if(this->tipo == PQT_DESTRUIR_OBJETO){
                            analizarPqtDestruirObjeto(bytes);
                        }else{
                            if(this->tipo == PQT_ABANDONAR){
                                analizarPqtDestruirAbandonar(bytes);
                            }else{
                                if (this->tipo == PQT_MANTENER_CONEXION) {
                                    analizarPqtMantenerConexion(bytes);
                                }else{
                                
                                /*
                                *Si no se cumple ninguna de las anteriores
                                *o es un mapa o no es ningun paquete valido.
                                */
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

size_t Paquete::nuevoPqtConfiguracion(Uint8 *buffer, const char *nombre, Uint8 tipo_juego, Sint8 mapa) {
    Uint8 *ptr;
    Uint8 tipo;

    ptr = buffer;
    tipo = PQT_CONFIGURACION;

    ptr = escribir(ptr, &tipo, sizeof(tipo));

    strcpy((char *)ptr, nombre);
    ptr += MAXTAM_MENSAJE + 1;

    ptr = escribir(ptr, &tipo_juego, sizeof(tipo_juego));
    ptr = escribir(ptr, &mapa, sizeof(mapa));

    return (size_t)(ptr - buffer);
}

size_t Paquete::nuevoPqtUnirse(Uint8 *buffer, const char *nombre) {
    Uint8 *ptr;
    Uint8 tipo;

    ptr = buffer;
    tipo = PQT_UNIRSE;

    ptr = escribir(ptr, &tipo, sizeof(tipo));
    strcpy((char *)ptr, nombre);
    ptr += MAXTAM_MENSAJE + 1;

    return (size_t)(ptr - buffer);
}

size_t Paquete::nuevoPqtConfirmacion(Uint8 *buffer, const char *msg) {
    Uint8 *ptr;
    Uint8 tipo;

    ptr = buffer;
    tipo = PQT_CONFIRMACION;

    ptr= escribir(ptr, &tipo, sizeof(tipo));
    strcpy((char *)ptr, msg);
    ptr += MAXTAM_MENSAJE + 1;

    return (size_t)(ptr - buffer);
}

size_t Paquete::nuevoPqtEvento(Uint8 *buffer, int pos_x, int pos_y, Uint8 evento, float velocidad) {
    Uint8 *ptr;
    Uint8 tipo;

    ptr = buffer;
    tipo = PQT_EVENTO;

    ptr = escribir(ptr, &tipo, sizeof(tipo));

    pos_x = htonl(pos_x);
    pos_y = htonl(pos_y);

    ptr = escribir(ptr, &pos_x, sizeof(pos_x));
    ptr = escribir(ptr, &pos_y, sizeof(pos_y));
    ptr = escribir(ptr, &evento, sizeof(evento));
    ptr = escribir(ptr, &velocidad, sizeof(velocidad));

    return (size_t)(ptr - buffer);
}

size_t Paquete::nuevoPqtAbandonar(Uint8 *buffer, const char *msg) {
    Uint8 *ptr;
    Uint8 tipo;

    ptr = buffer;
    tipo = PQT_ABANDONAR;

    ptr= escribir(ptr, &tipo, sizeof(tipo));
    strcpy((char *)ptr, msg);
    ptr += MAXTAM_MENSAJE + 1;

    return (size_t)(ptr - buffer);
}

size_t Paquete::nuevoPqtMantenerConexion(Uint8 *buffer, const char *msg) {
    Uint8 *ptr;
    Uint8 tipo;

    ptr = buffer;
    tipo = PQT_MANTENER_CONEXION;

    ptr= escribir(ptr, &tipo, sizeof(tipo));
    strcpy((char *)ptr, msg);
    ptr += MAXTAM_MENSAJE + 1;

    return (size_t)(ptr - buffer);
}