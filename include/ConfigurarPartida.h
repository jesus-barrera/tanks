#ifndef _CONFIGURAR_PARTIDA_H_
#define _CONFIGURAR_PARTIDA_H_

#include <SDL.h>
#include "Escena.h"
#include "SelectorMapa.h"
#include "Boton.h"
#include "Paquete.h"

#define NUM_OPT_VIDAS 4
#define CONFIG_TAM_BUFFER 1500

// Estados
enum {
    CONFIG_ST_CONFIGURAR,
    CONFIG_ST_ESPERANDO_JUGADOR,
    CONFIG_ST_CONFIRMACION
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
    static int options_vidas[NUM_OPT_VIDAS];
    static string etiquetas_botones[CONFIG_NUM_BTNS];
    
    int estado;

    MapaInfo *mapa_info;
    int opt_vidas;
    int modo_juego;

    SelectorMapa selector_mapa;

    Boton *botones[CONFIG_NUM_BTNS];
    Etiqueta *et_modo_juego;
    Etiqueta *et_mensaje;
    Boton *btns_vidas[NUM_OPT_VIDAS];
    
    Uint8 buffer[CONFIG_TAM_BUFFER];
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
    
    void enviarConfiguracion();
};

#endif