#ifdef _WIN32
    #define _WIN32_WINNT 0x501
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <poll.h>
    #include <netinet/in.h>
    #include <netdb.h>
#endif

#include <cerrno>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include "../include/network.h"

#ifdef _WIN32
    WSADATA wsa;

    bool Net_iniciarWSA() {
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
            printf("[Network] Error al inicializar WSA: %d", WSAGetLastError());
            return false;
        } else {
            return true;
        }
    }
#else
    typedef int SOCKET; 
#endif

static socklen_t addrlen_remoto;
static struct sockaddr_in dir_host_remoto;
static struct sockaddr_in dir_host_local;
static SOCKET socket_fd = -1;

/**
 * Incializa la dirección local sobre la cual se realiza el envío y recepción de paquetes. 
 *
 * Crea el socket y asocia la dirección del host con el puerto especificado. Inicializa a struct pollfd remoto
 * usado por Net_enviar y Net_recibir, por lo que debe llamarse antes que cualquiera de estas funciones.
 */
bool Net_iniciar(Uint16 puerto) {
    #ifdef _WIN32
        if (!Net_iniciarWSA()) return false;
    #endif

    // Crear socket
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("[Network] Error al crear socket");
        return false;
    }

    // Llenar dirección
    dir_host_local.sin_family = AF_INET;
    dir_host_local.sin_port = htons(puerto);
    dir_host_local.sin_addr.s_addr = INADDR_ANY;

    // Asociar dirección con socket
    if (bind(socket_fd, (struct sockaddr *) &dir_host_local, sizeof(dir_host_local)) == SOCKET_ERROR) {
        perror("[Network] Error al asociar direccion");
        return false;
    }

    /**
     * Convertir socket a no bloqueante
     */
    #ifdef _WIN32
        unsigned long mode = 1;
        ioctlsocket(socket_fd, FIONBIO, &mode);
    #else
        int flags;

        flags = fcntl(socket_fd, F_GETFL);
        flags = flags | O_NONBLOCK;

        fcntl(sockcket, F_SETFL, flags);
    #endif

    return true;
}

void Net_terminar() {
    #ifdef _WIN32
    closesocket(socket_fd);
    WSACleanup();
    #else
    close(socket_fd);
    #endif

    socket_fd = -1;
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
    int res;
    socklen_t addrlen;
    struct sockaddr_in src_addr;

    addrlen = sizeof(src_addr);
    
    res = recvfrom(socket_fd, (char *)buffer, bufflen, 0, (struct sockaddr *)&src_addr, &addrlen);

    if (res > 0) {
        if (recordar_host) {
            addrlen_remoto = addrlen;
            memcpy(&dir_host_remoto, &src_addr, addrlen);
        }

        return res;
    } else
    #ifdef _WIN32
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
    #else
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
    #endif
        /**
         * No hay datos que leer.
         */
    } else {
        perror("[Network] Error de lectura");
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
    int res;

    res = sendto(socket_fd, (char *)buffer, bufflen, 0, (struct sockaddr *)&dir_host_remoto, addrlen_remoto);
    
    if (res >= 0) {
        return res;
    } else
    #ifdef _WIN32
        if (WSAGetLastError() == WSAEWOULDBLOCK) {
    #else
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
    #endif
        /**
         * No se puede escribir en este momento.
         */
    } else {
        perror("[Network] Error de escritura");
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
        dir_host_remoto.sin_port = htons(NET_PORT);

        freeaddrinfo(results);
        return true;
    }
}

string obtenerNombreEquipo(){
    char hostname[MAXTAM_EQUIPO] = "";

    #ifdef _WIN32
    Net_iniciarWSA();
    #endif

    gethostname(hostname, MAXTAM_EQUIPO);

    return hostname;
}