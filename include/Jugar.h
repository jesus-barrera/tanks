#ifndef _JUGAR_H_INCLUDED_
#define _JUGAR_H_INCLUDED_

#include <SDL.h>

#include "Escena.h"
#include "Tanque.h"
#include "Base.h"
#include "Boton.h"
#include "Paquete.h"
#include "Temporizador.h"

#define PERDEDOR_MENSAJE "Perdiste!"
#define GANADOR_MENSAJE "Ganaste!"

struct MapaInfo;

class Jugador {
public:
    Tanque *tanque;
    Base *base;
    int num_vidas;
};

class Jugar: public Escena {
private:
    static const int TIEMPO_ESPERA = 3000;
    static const int TIEMPO_CONEXION_PERDIDA = 5000;
    static const int TAM_BUFFER = 1500;
    
    enum {
        ST_JUGAR,
        ST_ERROR,
        ST_FIN_PARTIDA
    };
    
    int estado;

    // Configuracion del juego
    int modo_net;
    int modo_juego;
    int num_vidas;

    Uint8 ganador;

    // Objetos
    Jugador *jugador;
    Jugador *oponente;

    Jugador jugador_1;
    Jugador jugador_2;

    // Objetos GUI
    Boton *boton_salir;
    Etiqueta *nombre_jugador;
    Etiqueta *nombre_oponente;
    Etiqueta *vidas_jugador;
    Etiqueta *vidas_oponente;
    Etiqueta *mensaje;

    Paquete paquete;
    Uint8 buffer[TAM_BUFFER];
    Temporizador enviado_temp;
    Temporizador recibido_temp;

    void mantenerConexion();
    void comprobarGanador();
    void abandonarPartida();
    void actualizarContadorVidas(Jugador *jugador, Etiqueta *contador);


public:
    // Modos de juego
    enum {
        MODO_JUEGO_VIDAS,
        MODO_JUEGO_BASE
    };

    // Rol en la red
    enum {
        MODO_SERVIDOR,
        MODO_CLIENTE
    };

    Jugar();
    ~Jugar();
    
    // Métodos de Escena
    void entrar();
    void actualizar();
    void renderizar();
    void manejarEvento(SDL_Event &evento);

    // Métodos para cargar el mapa
    bool cargarMapaPorId(Uint32 id);
    bool cargarMapaPorRuta(const char *ruta);

    void fijarModoJuego(int modo);
    void fijarNumVidas(int vidas);
    void fijarModoNet(int modo);

    void fijarNombreJugador(string nombre);
    void fijarNombreOponente(string nombre);
};

#endif