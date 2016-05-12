#ifndef _CONFIGURAR_PARTIDA_H_
#define _CONFIGURAR_PARTIDA_H_

#include <SDL.h>
#include "Escena.h"
#include "SelectorMapa.h"
#include "Boton.h"
#include "Paquete.h"
#include "Temporizador.h"

#define NUM_OPT_VIDAS 4
#define CONFIG_TAM_BUFFER 1500

// Estados
enum {
    CONFIG_ST_CONFIGURAR,
    CONFIG_ST_ESPERANDO_JUGADOR,
    CONFIG_ST_CONFIRMACION,
    CONFIG_ST_ERROR,
};

enum {
    CONFIG_BTN_LISTO,
    CONFIG_BTN_CANCELAR,
    CONFIG_BTN_JUEGO_A,
    CONFIG_BTN_JUEGO_B,
    CONFIG_NUM_BTNS
};

class ConfigurarPartida: public Escena {
private:
    static const int MAX_CONFIRM_TIEMPO = 5000;
    static int options_vidas[NUM_OPT_VIDAS];
    static string etiquetas_botones[CONFIG_NUM_BTNS];
    
    string nombre_host;

    int estado;

    MapaInfo *mapa_info;
    int opt_vidas;
    int modo_juego;

    SelectorMapa selector_mapa;

    Boton *botones[CONFIG_NUM_BTNS];
    Etiqueta *et_modo_juego;
    Etiqueta *et_mensaje;
    Etiqueta *et_nombre_host;

    Boton *btns_vidas[NUM_OPT_VIDAS];
    
    Uint8 buffer[CONFIG_TAM_BUFFER];
    Paquete paquete;
    Temporizador temp;

public:
    ConfigurarPartida();
    ~ConfigurarPartida();

    void entrar();
    void actualizar();
    void renderizar();
    void manejarEvento(SDL_Event &evento);

    void esperarManejarEvento(SDL_Event &evento);
    void configManejarEvento(SDL_Event &evento);
    
    void enviarConfiguracion();
    void enviarMapa();
};

#endif