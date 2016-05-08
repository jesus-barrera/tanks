#include "../include/ConfigurarPartida.h"
#include "../include/Escenario.h"
#include "../include/Editor.h"
#include "../include/Jugar.h"
#include "../include/utiles.h"
#include "../include/network.h"

int ConfigurarPartida::options_vidas[NUM_OPT_VIDAS] = {1, 3, 5, 10};
string ConfigurarPartida::etiquetas_botones[CONF_NUM_BTNS] = {"Listo", "Cancelar", "Por vidas", "Por base"};

/**
 * Crear y posicionar objetos de la escena.
 */
ConfigurarPartida::ConfigurarPartida() {
    int x_offset;
    int y_offset;
    int btn_sep;
    
    // Crear botones
    for (int i = 0; i < CONF_NUM_BTNS; i++) {
        botones[i] = new Boton(etiquetas_botones[i], 0, 0);
    }

    /**
     * Posicionar botones del panel derecho
     */
    btn_sep = DEFAULT_FONT_SIZE + 10; 
    x_offset = VENTANA_PADDING;
    y_offset = VENTANA_ALTO - (btn_sep * 2 + 10);

    botones[CONF_BTN_LISTO]->fijarPosicion(x_offset,  y_offset);
    botones[CONF_BTN_LISTO]->setViewport(&vista_estatus);
    botones[CONF_BTN_CANCELAR]->fijarPosicion(x_offset, (y_offset += btn_sep));
    botones[CONF_BTN_CANCELAR]->setViewport(&vista_estatus);

    et_mensaje = new Etiqueta("", VENTANA_PADDING, VENTANA_PADDING, DEFAULT_FONT_SIZE, COLOR_BLANCO);
    
    /**
     * Posicionar objetos de la vista de juego
     */
    y_offset = VENTANA_PADDING;
    et_modo_juego = new Etiqueta("Seleccionar el modo de juego", x_offset, y_offset, DEFAULT_FONT_SIZE, COLOR_AZUL);
    
    y_offset += btn_sep;
    botones[CONF_BTN_JUEGO_A]->fijarPosicion(x_offset + 20,  y_offset);

    // Posicionar opciones de vidas en la misma linea
    for (int i = 0, x_offset = 175; i < NUM_OPT_VIDAS; i++, x_offset += 25) {
        btns_vidas[i] = new Boton(to_string(options_vidas[i]), x_offset, y_offset);
    }

    botones[CONF_BTN_JUEGO_B]->fijarPosicion(x_offset + 20,  (y_offset += btn_sep));
    
    selector_mapa.fijarPosicion(x_offset, y_offset += btn_sep * 2);
}

/**
 * Liberar memoria reservada.
 */
ConfigurarPartida::~ConfigurarPartida() {
    delete(et_mensaje);
    delete(et_modo_juego);

    Boton::eliminarBotones(botones, CONF_NUM_BTNS);
    Boton::eliminarBotones(btns_vidas, NUM_OPT_VIDAS);

    Net_terminar();
}

/**
 * Se llama antes de entrar a la escena. Reinicia los objetos.
 */
void ConfigurarPartida::entrar() {
    Escenario::limpiarMapa();
    selector_mapa.cargarMapasInfo();

    botones[CONF_BTN_JUEGO_A]->estaSeleccionado(false);
    botones[CONF_BTN_JUEGO_B]->estaSeleccionado(false);

    // Reiniciar configuracion del juego
    opt_vidas = -1;
    modo_juego = -1;
    mapa_info = NULL;

    estado = CONF_ST_CONFIG_PARTIDA;
}

void ConfigurarPartida::actualizar() { }

void ConfigurarPartida::renderizar() {
    SDL_RenderSetViewport(renderer_principal, &vista_juego);
    
    Escenario::renderizar();
    renderizarCapaGris();

    if (estado == CONF_ST_CONFIG_PARTIDA) {
        selector_mapa.actualizar();
        et_modo_juego->renderizar();
        
        Boton::renderizarBotones(botones, CONF_NUM_BTNS);
    
        if (modo_juego == MODO_JUEGO_VIDAS) Boton::renderizarBotones(btns_vidas, NUM_OPT_VIDAS);
    } else if (estado == CONF_ST_ESPERANDO_JUGADOR) {
        if (Net_recibir(buffer, CONF_TAM_BUFFER)) {
            paquete.analizar(buffer);

            if (paquete.tipo == PQT_UNIRSE) {
                et_mensaje->fijarTexto((string)paquete.nombre);
            }
        }

        SDL_RenderSetViewport(renderer_principal, &vista_juego);
        et_mensaje->renderizar();
        
        SDL_RenderSetViewport(renderer_principal, &vista_estatus);
        botones[CONF_BTN_CANCELAR]->renderizar();
    }
}

void ConfigurarPartida::manejarEvento(SDL_Event &evento) {
    if (estado == CONF_ST_CONFIG_PARTIDA) {
        configManejarEvento(evento);
    } else if (estado == CONF_ST_ESPERANDO_JUGADOR) {
        esperarManejarEvento(evento);
    }
}

void ConfigurarPartida::esperarManejarEvento(SDL_Event &evento) {
    if (evento.type == SDL_MOUSEBUTTONDOWN && botones[CONF_BTN_CANCELAR]->isMouseOver()) {
        Net_terminar();

        estado = CONF_ST_CONFIG_PARTIDA;
    }
}

void ConfigurarPartida::configManejarEvento(SDL_Event &evento) {
    if (evento.type == SDL_MOUSEBUTTONDOWN) {
        MapaInfo *mapa = selector_mapa.obtenerMapaSelecInfo();

        if (mapa && mapa != mapa_info) {
            Editor::cargarMapa(mapa->ruta, NULL, NULL, NULL, NULL);
            mapa_info = mapa;
        } else {
            switch (Boton::obtenerBotonSeleccionado(botones, CONF_NUM_BTNS)) {
                case CONF_BTN_LISTO:
                    if (mapa_info && (modo_juego == MODO_JUEGO_BASE || 
                        (modo_juego == MODO_JUEGO_VIDAS && opt_vidas != -1))) {

                        Jugar::mapa_info = mapa_info;
                        Jugar::modo_juego = modo_juego;
                        Jugar::num_vidas = options_vidas[opt_vidas];

                        estado = CONF_ST_ESPERANDO_JUGADOR;

                        if (Net_iniciar()) {
                            et_mensaje->fijarTexto("Esperando jugador ...");
                        } else {
                            et_mensaje->fijarTexto("Ocurrio un error al inicar conexion");
                        }
                    }
                    break;
                case CONF_BTN_CANCELAR:
                    irAEscena("menu");
                    break;
                case CONF_BTN_JUEGO_A:
                    modo_juego = MODO_JUEGO_VIDAS;
                    botones[CONF_BTN_JUEGO_A]->estaSeleccionado(true);
                    botones[CONF_BTN_JUEGO_B]->estaSeleccionado(false);
                    break;
                case CONF_BTN_JUEGO_B:
                    modo_juego = MODO_JUEGO_BASE;
                    botones[CONF_BTN_JUEGO_B]->estaSeleccionado(true);
                    botones[CONF_BTN_JUEGO_A]->estaSeleccionado(false);
                    break;
                default: { 
                    // Revisar si se eligió un numero de vidas                   
                    int opt = Boton::obtenerBotonSeleccionado(btns_vidas, NUM_OPT_VIDAS);
 
                    if (opt != -1 && opt != opt_vidas) {
                        if (opt_vidas != -1) btns_vidas[opt_vidas]->estaSeleccionado(false);
                    
                        btns_vidas[opt]->estaSeleccionado(true);
                        opt_vidas = opt;
                    }
                }
            }
        }
    }
}