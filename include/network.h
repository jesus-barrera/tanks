#ifndef _NETWORK_H_INCLUDE_
#define _NETWORK_H_INCLUDE_

#define NET_POLL_TIMEOUT 40
#define NET_PORT 2345

#include <SDL.h>

/**
 * Incializa la dirección local
 */
bool Net_iniciar();

void Net_terminar();

/**
 * Lee los bytes disponibles
 */
int Net_recibir(Uint8 *buffer, int bufflen);

/**
 * 
 */
int Net_enviar(Uint8 *buffer, int bufflen);

#endif //_NETWORK_H_INCLUDE_