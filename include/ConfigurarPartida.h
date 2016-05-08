#ifndef _CONFIGURAR_PARTIDA_H_
#define _CONFIGURAR_PARTIDA_H_

#include <SDL.h>
#include "Escena.h"
#include "SelectorMapa.h"
#include "Boton.h"
#include "Paquete.h"

#define NUM_OPT_VIDAS 4
#define CONF_TAM_BUFFER 1500

// Estados
enum {
    CONF_ST_ESPERANDO_JUGADOR,
    CONF_ST_CONFIG_PARTIDA
};

enum {
    CONF_BTN_LISTO,
    CONF_BTN_CANCELAR,
    CONF_BTN_JUEGO_A,
    CONF_BTN_JUEGO_B,
    CONF_NUM_BTNS
};

class ConfigurarPartida: public Escena {
private:
    static int options_vidas[NUM_OPT_VIDAS];
    static string etiquetas_botones[CONF_NUM_BTNS];
    
    int estado;

    MapaInfo *mapa_info;
    int opt_vidas;
    int modo_juego;

    SelectorMapa selector_mapa;

    Boton *botones[CONF_NUM_BTNS];
    Etiqueta *et_modo_juego;
    Etiqueta *et_mensaje;
    Boton *btns_vidas[NUM_OPT_VIDAS];
    
    Uint8 buffer[CONF_TAM_BUFFER];
    Paquete paquete;

public:
    ConfigurarPartida();
    ~ConfigurarPartida();

    void entrar();
    void actualizar();
    void renderizar();
    void manejarEvento(SDL_Event &evento);

    void esperarManejarEvento(SDL_Event &evento);
    void configManejarEvento(SDL_Event &evento);
};

#endif