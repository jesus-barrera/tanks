#ifndef _JUGAR_H_INCLUDED_
#define _JUGAR_H_INCLUDED_

#include <SDL.h>

#include "Escena.h"
#include "Tanque.h"
#include "Base.h"
#include "Boton.h"

struct MapaInfo;

class Jugador {
public:
    string nombre;
    Tanque *tanque;
    Base *base;
    int num_vidas;
};

class Jugar: public Escena {
private:
    // Configuracion del juego
    static int modo_juego;
    static int num_vidas;
        
    // Objetos
    static Tanque *jugador;
    static Tanque *tanque_j1;
    static Tanque *tanque_j2;
    static Base *base_j1;
    static Base *base_j2;

    // Objetos GUI
    static Boton *boton_salir;
public:
    // Modos de juego
    enum {
        MODO_JUEGO_VIDAS,
        MODO_JUEGO_BASE
    };

    static void inicializar();
    static void liberarMemoria();
    
    // Métodos de Escena
    void entrar();
    void actualizar();
    void renderizar();
    void manejarEvento(SDL_Event &evento);

    // Métodos para cargar el mapa
    static bool cargarMapaPorId(Uint32 id);
    static bool cargarMapaPorRuta(const char *ruta);

    static void fijarModoJuego(int modo);
    static void fijarNumVidas(int vidas);
};

#endif