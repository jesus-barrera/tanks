#ifndef _NETWORK_H_INCLUDE_
#define _NETWORK_H_INCLUDE_

#define NET_POLL_TIMEOUT 40
#define NET_PORT 2345

#define MAXTAM_USUARIO 20
#define MAXTAM_EQUIPO 20

#include <SDL.h>
#include <string>

using namespace std;

/**
 * Incializa la dirección local
 */
bool Net_iniciar(Uint16 puerto = NET_PORT);

void Net_terminar();

/**
 * Lee los bytes disponibles
 */
int Net_recibir(Uint8 *buffer, int bufflen);

/**
 * 
 */
int Net_enviar(Uint8 *buffer, int bufflen);

/**
 * Resuelve el nombre de host a una dirección ip.
 */
int Net_resolverHost(string nombre_host);

/**
 * Retorna el nombre del equipo donde esta la aplicacion
 */
string obtenerNombreEquipo();

/**
 * Retorna el nombre de usuario
 */
string obtenerNombreUsuario();

#endif //_NETWORK_H_INCLUDE_