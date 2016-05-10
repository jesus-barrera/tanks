#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../include/network.h"

socklen_t addrlen_remoto;
struct sockaddr_in6 dir_host_remoto;
struct sockaddr_in6 dir_host_local;
struct pollfd remoto;

/**
 * Incializa la dirección local
 */
bool Net_iniciar(Uint16 puerto) {
    int socket_fd;

    // Crear socket
    if ((socket_fd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1) {
        perror("[Debug] Error al crear socket");
        return false;
    }

    // Llenar dirección
    dir_host_local.sin6_family = AF_INET6;
    dir_host_local.sin6_port = htons(puerto);
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
 * Envia el contenido del buffer
 */
int Net_enviar(Uint8 *buffer, int bufflen) {
    int res = poll(&remoto, 1, NET_POLL_TIMEOUT);

    if (res > 0) {
        if ((remoto.revents & POLLOUT) != 0) {
            res = sendto(remoto.fd, buffer, bufflen, 0, (struct sockaddr *)&dir_host_remoto, addrlen_remoto);

            if (res < 0) {
                perror("[Debug] Error de escritura");
            } else {
                return res;
            }
        }
    } else if (res < 0){
        perror("[Debug] Error en poll");
    }

    return 0;
}

int Net_resolverHost(string nombre_host) {
    int res;
    struct addrinfo hints, *results;
    memset(&hints, 0, sizeof(hints));
    if((res = getaddrinfo(nombre_host.c_str(), NULL, &hints, &results)) != 0){
        perror("Error en el getaddrinfo...");

        return false;
    } else {
        addrlen_remoto = results->ai_addrlen;
        memcpy(&dir_host_remoto, results->ai_addr, addrlen_remoto);
        dir_host_remoto.sin6_port = htons(NET_PORT);

        freeaddrinfo(results);
        return true;
    }
}


string obtenerNombreEquipo(){
    char hostname[MAXTAM_EQUIPO];
    string retorno;
    gethostname(hostname, MAXTAM_EQUIPO);
    //cout<<"Hostname: "<<hostname<<endl;
    retorno=hostname;
    return retorno;
}

string obtenerNombreUsuario(){
    char username[MAXTAM_USUARIO];
    string retorno;
    getlogin_r(username, MAXTAM_USUARIO);
    //cout<<"Username: "<<username<<endl;
    retorno=username;
    return retorno;
}
