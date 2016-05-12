#include <cerrno>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../include/network.h"

static socklen_t addrlen_remoto;
static struct sockaddr_in6 dir_host_remoto;
static struct sockaddr_in6 dir_host_local;
static struct pollfd remoto;

/**
 * Incializa la dirección local sobre la cual se realiza el envío y recepción de paquetes. 
 *
 * Crea el socket y asocia la dirección del host con el puerto especificado. Inicializa a struct pollfd remoto
 * usado por Net_enviar y Net_recibir, por lo que debe llamarse antes que cualquiera de estas funciones.
 */
bool Net_iniciar(Uint16 puerto) {
    int socket_fd;

    // Crear socket
    if ((socket_fd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1) {
        perror("[Network] Error al crear socket");
        return false;
    }

    // Llenar dirección
    dir_host_local.sin6_family = AF_INET6;
    dir_host_local.sin6_port = htons(puerto);
    dir_host_local.sin6_addr = in6addr_any;

    // Asociar dirección con socket
    if (bind(socket_fd, (struct sockaddr *) &dir_host_local, sizeof(dir_host_local)) == -1) {
        perror("[Network] Error al asociar direccion");
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
 * Revisa si hay datos para leer y los guarda en el buffer. 
 *
 * Si recordar_host es true, se aceptan paquetes de cualquier host y su dirección se convierte en la
 * dirección remota (dir_host_remoto) por defecto, utilizada por Net_enviar como dirección de destino.
 *
 * Si recordar_host es false, asume que ya se conoce un host remoto (dir_host_remoto).
 *
 * Asume que Net_iniciar ha sido llamada previamente con éxito.
 */
int Net_recibir(Uint8 *buffer, int bufflen, bool recordar_host) {
    socklen_t addrlen;
    struct sockaddr_in6 src_addr;
    
    int res = poll(&remoto, 1, NET_POLL_TIMEOUT);

    if (res > 0) {      
        if ((remoto.revents & POLLIN) != 0) {
            addrlen = sizeof(src_addr);
            
            res = recvfrom(remoto.fd, buffer, bufflen, 0, (struct sockaddr *)&src_addr, &addrlen);

            if (res <= 0) {
                perror("[Network] Error de lectura");
            } else {
                if (recordar_host) {
                    addrlen_remoto = addrlen;
                    memcpy(&dir_host_remoto, &src_addr, addrlen);
                }

                return res;
            }
        }
    } else if (res < 0) {
        perror("[Network] Error en poll");
    }

    return 0;
}

/**
 * Envia bufflen bytes de buffer a dir_host_remoto. 
 * 
 * Para que dir_host_remoto contenga una dirección válida, debió llamarse previamente con éxito
 * Net_recibir con recordar_host = true, o bien Net_resolverHost.
 *
 * Asume que Net_iniciar ha sido llamada previamente con éxito.
 */
int Net_enviar(Uint8 *buffer, int bufflen) {
    int res = poll(&remoto, 1, NET_POLL_TIMEOUT);

    if (res > 0) {
        if ((remoto.revents & POLLOUT) != 0) {
            res = sendto(remoto.fd, buffer, bufflen, 0, (struct sockaddr *)&dir_host_remoto, addrlen_remoto);

            if (res < 0) {
                perror("[Network] Error de escritura");
            } else {
                return res;
            }
        }
    } else if (res < 0){
        perror("[Network] Error en poll");
    }

    return 0;
}

/**
 * Intenta resolver el nombre del host a una dirección IP utilizando el puerto por defecto.
 *
 * En caso de éxito guarda la dirección resuelta en dir_host_remoto para usarse en posteriores
 * llamadas a Net_enviar.
 */
int Net_resolverHost(string nombre_host) {
    int res;
    struct addrinfo hints, *results;
    memset(&hints, 0, sizeof(hints));
    
    if((res = getaddrinfo(nombre_host.c_str(), NULL, &hints, &results)) != 0){
        perror("[Network] Error en el getaddrinfo...");

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
    retorno=hostname;
    return retorno;
}

string obtenerNombreUsuario(){
    char username[MAXTAM_USUARIO];
    string retorno;
    getlogin_r(username, MAXTAM_USUARIO);
    retorno=username;
    return retorno;
}
