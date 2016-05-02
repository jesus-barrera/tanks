#include "../include/ConfigurarPartida.h"
#include "../include/Escenario.h"
#include "../include/Editor.h"
#include "../include/Jugar.h"
#include "../include/utiles.h"

int ConfigurarPartida::options_vidas[NUM_OPT_VIDAS] = {1, 3, 5, 10};

/**
 * Crear y posicionar objetos de la escena.
 */
ConfigurarPartida::ConfigurarPartida() {
    int btn_x;
    int btn_y;
    int btn_sep;

    // Posicionar botones
    btn_sep = DEFAULT_FONT_SIZE + 10; 
    btn_x = VENTANA_PADDING;
    btn_y = VENTANA_ALTO - (btn_sep * 2 + 10);
    
    btn_comenzar = new Boton("Comenzar", btn_x,  btn_y);
    btn_comenzar->setViewport(&vista_estatus);
    btn_cancelar = new Boton("Cancelar", btn_x, btn_y +  btn_sep);
    btn_cancelar->setViewport(&vista_estatus);

    et_modo_juego = new Etiqueta("Seleccionar el modo de juego", 0, 0, DEFAULT_FONT_SIZE, COLOR_AZUL);
    btn_modo_a = new Boton("Por vidas", 0, 0);
    btn_modo_b = new Boton("Por base", 0, 0);

    for (int i = 0; i < NUM_OPT_VIDAS; i++) {
        btns_vidas[i] = new Boton(to_string(options_vidas[i]), 0, 0);
    }
}

/**
 * Liberar memoria reservada.
 */
ConfigurarPartida::~ConfigurarPartida() {
    delete(btn_comenzar);
    delete(btn_cancelar);
    delete(btn_modo_a);
    delete(btn_modo_b);
    delete(et_modo_juego);

    for (int i = 0; i < NUM_OPT_VIDAS; i++) {
        delete(btns_vidas[i]);
        btns_vidas[i] = NULL;
    }
}

/**
 * Se llama antes de entrar a la escena. Reinicia los objetos.
 */
void ConfigurarPartida::entrar() {
    int y_offset;
    int btn_sep;

    Escenario::limpiarMapa();
    selector_mapa.cargarMapasInfo();

    btn_modo_a->estaSeleccionado(false);
    btn_modo_b->estaSeleccionado(false);


    // Reiniciar configuracion del juego
    opt_vidas = -1;
    modo_juego = -1;
    mapa_info  = NULL;

    /**
     * Las opciones de modo de juego deben posicionarse debajo del selector de mapa.
     */
    btn_sep = DEFAULT_FONT_SIZE + 10;
    y_offset = selector_mapa.obtenerBordeInferior() + 10;
    
    et_modo_juego->fijarPosicion(VENTANA_PADDING, y_offset);
    
    y_offset += btn_sep;
    btn_modo_a->fijarPosicion(VENTANA_PADDING + 20,  y_offset);

    // Posicionar opciones de vidas en la misma linea
    for (int i = 0, x_offset = 175; i < NUM_OPT_VIDAS; i++, x_offset += 25) {
        btns_vidas[i]->fijarPosicion(x_offset,  y_offset);
        btns_vidas[i]->estaSeleccionado(false);
    }
    
    btn_modo_b->fijarPosicion(VENTANA_PADDING + 20,  (y_offset += btn_sep));
}

/**
 * Llamada en cada frame. Renderiza los objetos.
 */
void ConfigurarPartida::actualizar() {
    SDL_RenderSetViewport(renderer_principal, &vista_juego);
    
    Escenario::renderizar();
    renderizarCapaGris();
    
    selector_mapa.actualizar();
    
    et_modo_juego->renderizar();
    btn_modo_a->renderizar();
    btn_modo_b->renderizar();

    if (modo_juego == MODO_JUEGO_VIDAS) {
        for (int i = 0; i < NUM_OPT_VIDAS; i++) {
            btns_vidas[i]->renderizar();
        }
    }

    SDL_RenderSetViewport(renderer_principal, &vista_estatus);
    btn_comenzar->renderizar();
    btn_cancelar->renderizar();
}

/**
 * Manejador de eventos. Ubica sobre que botones se hace click y ajusta los
 * valores de la partida.
 */
void ConfigurarPartida::manejarEvento(SDL_Event &evento) {
    if (evento.type == SDL_MOUSEBUTTONDOWN) {
        MapaInfo *mapa = selector_mapa.obtenerMapaSelecInfo();

        if (mapa) {
            if (mapa != mapa_info) {
                Editor::cargarMapa(mapa->ruta, NULL, NULL, NULL, NULL);

                mapa_info = mapa;   
            }
        } else {
            if (btn_cancelar->isMouseOver()) {
                irAEscena("menu");
            } else if (btn_comenzar->isMouseOver()) {
                if (mapa_info && (modo_juego == MODO_JUEGO_BASE || 
                    (modo_juego == MODO_JUEGO_VIDAS && opt_vidas != -1))) {

                    Jugar::mapa_info = mapa_info;
                    Jugar::modo_juego = modo_juego;
                    Jugar::num_vidas = options_vidas[opt_vidas];

                    irAEscena("jugar");
                }
            } else if (btn_modo_a->isMouseOver()) {
                modo_juego = MODO_JUEGO_VIDAS;
                btn_modo_a->estaSeleccionado(true);
                btn_modo_b->estaSeleccionado(false);
            } else if (btn_modo_b->isMouseOver()) {
                modo_juego = MODO_JUEGO_BASE;
                btn_modo_b->estaSeleccionado(true);
                btn_modo_a->estaSeleccionado(false);
            } else {
                int opt;
                opt = Boton::obtenerBotonSeleccionado(btns_vidas, NUM_OPT_VIDAS);

                if (opt != -1) {
                    if (opt != opt_vidas) {
                        if (opt_vidas != -1) btns_vidas[opt_vidas]->estaSeleccionado(false);
                    
                        btns_vidas[opt]->estaSeleccionado(true);
                        opt_vidas = opt;
                    }
                }
            }
        }
    }
}
