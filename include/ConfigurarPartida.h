#ifndef _CONFIGURAR_PARTIDA_H_
#define _CONFIGURAR_PARTIDA_H_

#include <SDL.h>
#include "Escena.h"
#include "SelectorMapa.h"
#include "Boton.h"

#define NUM_OPT_VIDAS 4

class ConfigurarPartida: public Escena {
private:
    static int options_vidas[NUM_OPT_VIDAS];

    MapaInfo *mapa_info;
    int opt_vidas;
    int modo_juego;

    SelectorMapa selector_mapa;

    Boton *btn_comenzar;
    Boton *btn_cancelar;
    Boton *btn_modo_a;
    Boton *btn_modo_b;
    Etiqueta *et_modo_juego;
    Boton *btns_vidas[NUM_OPT_VIDAS];

public:
    ConfigurarPartida();
    ~ConfigurarPartida();

    void entrar();
    void actualizar();
    void manejarEvento(SDL_Event &evento);
};

#endif