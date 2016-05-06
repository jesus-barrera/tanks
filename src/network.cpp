#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>
#include <arpa/inet.h>
#include "../include/network.h"

socklen_t addrlen_remoto;
struct sockaddr_in6 dir_host_remoto;
struct sockaddr_in6 dir_host_local;
struct pollfd remoto;

/**
 * Incializa la dirección local
 */
bool Net_iniciar() {
    int socket_fd;

    // Crear socket
    if ((socket_fd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1) {
        perror("[Debug] Error al crear socket");
        return false;
    }

    // Llenar dirección
    dir_host_local.sin6_family = AF_INET6;
    dir_host_local.sin6_port = htons(NET_PORT);
    dir_host_local.sin6_addr = in6addr_any;

    // Asociar dirección con socket
    if (bind(socket_fd, (struct sockaddr *) &dir_host_local, sizeof(dir_host_local)) == -1) {
        perror("[Debug] Error en bind");
        return false;
    }

    remoto.fd = socket_fd;
    remoto.events = POLLIN | POLLOUT;

    return true;
}

void Net_terminar() {
    close(remoto.fd);
    remoto.fd = -1;
}

/**
 * Lee los bytes disponibles
 */
int Net_recibir(Uint8 *buffer, int bufflen) {
    int res = poll(&remoto, 1, NET_POLL_TIMEOUT);

    if (res > 0) {      
        if ((remoto.revents & POLLIN) != 0) {
            addrlen_remoto = sizeof(dir_host_remoto);
            
            res = recvfrom(remoto.fd, buffer, bufflen, 0, (struct sockaddr *)&dir_host_remoto, &addrlen_remoto);

            if (res <= 0) {
                perror("[Debug] Error de lectura");
            } else {
                return res;
            }
        }
    } else if (res < 0) {
        perror("[Debug] Error en poll");
    }

    return 0;
}

/**
 * 
 */
int Net_enviar(Uint8 *buffer, int bufflen) {
    return false;
}