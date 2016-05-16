#include "../include/ConfigurarPartida.h"
#include "../include/Escenario.h"
#include "../include/Editor.h"
#include "../include/Jugar.h"
#include "../include/utiles.h"
#include "../include/Musica.h"
#include "../include/network.h"

int ConfigurarPartida::options_vidas[NUM_OPT_VIDAS] = {1, 3, 5, 10};
string ConfigurarPartida::etiquetas_botones[CONFIG_NUM_BTNS] = {"Listo", "Cancelar", "Por vidas", "Por base"};

/**
 * Crear y posicionar objetos de la escena.
 */
ConfigurarPartida::ConfigurarPartida() {
    int x_offset;
    int y_offset;
    int btn_sep;

    nombre_host = obtenerNombreEquipo();

    // Crear botones
    for (int i = 0; i < CONFIG_NUM_BTNS; i++) {
        botones[i] = new Boton(etiquetas_botones[i], 0, 0);
    }

    /**
     * Posicionar botones del panel derecho
     */
    btn_sep = DEFAULT_FONT_SIZE + 10;
    x_offset = VENTANA_PADDING;
    y_offset = VENTANA_ALTO - (btn_sep * 2 + 10);

    botones[CONFIG_BTN_LISTO]->fijarPosicion(x_offset,  y_offset);
    botones[CONFIG_BTN_LISTO]->setViewport(&vista_estatus);

    botones[CONFIG_BTN_CANCELAR]->fijarPosicion(x_offset, (y_offset += btn_sep));
    botones[CONFIG_BTN_CANCELAR]->setViewport(&vista_estatus);

    et_mensaje = new Etiqueta("", VENTANA_PADDING, VENTANA_PADDING);
    et_nombre_host = new Etiqueta(nombre_host, VENTANA_PADDING, VENTANA_PADDING);

    /**
     * Posicionar objetos de la vista de juego
     */
    y_offset = VENTANA_PADDING;
    et_modo_juego = new Etiqueta("Seleccionar el modo de juego", x_offset, y_offset, DEFAULT_FONT_SIZE, COLOR_AZUL);

    y_offset += btn_sep;
    botones[CONFIG_BTN_JUEGO_A]->fijarPosicion(x_offset + 20,  y_offset);

    // Posicionar opciones de vidas en la misma linea
    for (int i = 0, x_offset = 175; i < NUM_OPT_VIDAS; i++, x_offset += 25) {
        btns_vidas[i] = new Boton(inttostr(options_vidas[i]), x_offset, y_offset);
    }

    botones[CONFIG_BTN_JUEGO_B]->fijarPosicion(x_offset + 20,  (y_offset += btn_sep));

    selector_mapa.fijarPosicion(x_offset, y_offset += btn_sep * 2);

    opt_vidas = -1;
}

/**
 * Liberar memoria reservada.
 */
ConfigurarPartida::~ConfigurarPartida() {
    delete(et_mensaje);
    delete(et_modo_juego);
    delete(et_nombre_host);

    Boton::eliminarBotones(botones, CONFIG_NUM_BTNS);
    Boton::eliminarBotones(btns_vidas, NUM_OPT_VIDAS);

    Net_terminar();
}

/**
 * Se llama antes de entrar a la escena. Reinicia los objetos.
 */
void ConfigurarPartida::entrar() {
//    cambiarMusicaFondo(MusicaFondoCrearMapa);
//    ReproducirMusicaFondo();

    Escenario::limpiarMapa();
    selector_mapa.cargarMapasInfo();

    botones[CONFIG_BTN_JUEGO_A]->estaSeleccionado(false);
    botones[CONFIG_BTN_JUEGO_B]->estaSeleccionado(false);

    // Reiniciar configuracion del juego
    if (opt_vidas != -1) btns_vidas[opt_vidas]->estaSeleccionado(false);

    opt_vidas  = -1;
    modo_juego = -1;
    mapa_info  = NULL;

    estado = CONFIG_ST_CONFIGURAR;
}

void ConfigurarPartida::actualizar() {
    switch (estado) {
        case CONFIG_ST_ESPERANDO_JUGADOR:
        if (Net_recibir(buffer, CONFIG_TAM_BUFFER, true)) {
            paquete.analizar(buffer);

            if (paquete.tipo == PQT_UNIRSE) {
                string nombre = (string)paquete.nombre;

                ((Jugar *)obtenerEscena("jugar"))->fijarNombreOponente(nombre);
                et_mensaje->fijarTexto("Conectando con \"" + nombre + "\" ...");

                enviarConfiguracion();

                temp.iniciar();
                estado = CONFIG_ST_CONFIRMACION;
            }
        }
        break;

        case CONFIG_ST_CONFIRMACION:
        /**
         * Esperar por confirmación de la configuración
         */
        if (Net_recibir(buffer, CONFIG_TAM_BUFFER)) {
            paquete.analizar(buffer);

            if (paquete.tipo == PQT_CONFIRMACION) {
                cout << "[Protocolo] Recibido: \"" << paquete.mensaje << "\"" << endl;
                if (!mapa_info->en_juego) {
                    enviarMapa();
                }

                irAEscena("jugar");
            } else {
                cout << "[Protocolo] Paquete descartado" << endl;
            }

        } else if (temp.obtenerTiempo() > MAX_CONFIRM_TIEMPO) {
            et_mensaje->fijarTexto("No se pudo crear la partida");

            cout << "[Protocolo] Tiempo de confirmacion expirado" << endl;
            estado = CONFIG_ST_ERROR;
        }
        break;
    }
}

void ConfigurarPartida::renderizar() {
    SDL_RenderSetViewport(renderer_principal, &vista_juego);

    Escenario::renderizar();
    renderizarCapaGris();

    if (estado == CONFIG_ST_CONFIGURAR) {
        selector_mapa.renderizar();
        et_modo_juego->renderizar();

        Boton::renderizarBotones(botones, CONFIG_NUM_BTNS);

        if (modo_juego == Jugar::MODO_JUEGO_VIDAS) Boton::renderizarBotones(btns_vidas, NUM_OPT_VIDAS);

        SDL_RenderSetViewport(renderer_principal, &vista_estatus);

        et_nombre_host->renderizar();
    } else {
        SDL_RenderSetViewport(renderer_principal, &vista_juego);
        et_mensaje->renderizar();

        SDL_RenderSetViewport(renderer_principal, &vista_estatus);
        botones[CONFIG_BTN_CANCELAR]->renderizar();
    }
}

void ConfigurarPartida::manejarEvento(SDL_Event &evento) {
    if (estado == CONFIG_ST_CONFIGURAR) {
        configManejarEvento(evento);
    } else {
        esperarManejarEvento(evento);
    }
}

void ConfigurarPartida::esperarManejarEvento(SDL_Event &evento) {
    if (evento.type == SDL_MOUSEBUTTONDOWN && botones[CONFIG_BTN_CANCELAR]->isMouseOver()) {
        Net_terminar();

        estado = CONFIG_ST_CONFIGURAR;
    }
}

void ConfigurarPartida::configManejarEvento(SDL_Event &evento) {
    if (evento.type == SDL_MOUSEBUTTONDOWN) {
        MapaInfo *mapa = selector_mapa.obtenerMapaSelecInfo();

        if (mapa && mapa != mapa_info) {
            Editor::cargarMapa(mapa->ruta, NULL, NULL, NULL, NULL);
            mapa_info = mapa;

        } else {
            switch (Boton::obtenerBotonSeleccionado(botones, CONFIG_NUM_BTNS)) {
                case CONFIG_BTN_LISTO:
                    ReproducirSonido(Snd_Click_boton, 100, 0, 0);
                    if (mapa_info && (modo_juego == Jugar::MODO_JUEGO_BASE ||
                        (modo_juego == Jugar::MODO_JUEGO_VIDAS && opt_vidas != -1))) {

                        Jugar *jugar = (Jugar *)obtenerEscena("jugar");

                        jugar->cargarMapaPorRuta(mapa_info->ruta);
                        jugar->fijarModoJuego(modo_juego);
                        jugar->fijarModoNet(Jugar::MODO_SERVIDOR);
                        jugar->fijarNombreJugador(nombre_host);

                        if (modo_juego == Jugar::MODO_JUEGO_VIDAS) {
                            jugar->fijarNumVidas(options_vidas[opt_vidas]);
                        }

                        estado = CONFIG_ST_ESPERANDO_JUGADOR;

                        if (Net_iniciar()) {
                            et_mensaje->fijarTexto("Esperando jugador ...");
                        } else {
                            et_mensaje->fijarTexto("Ocurrio un error al inicar conexion");
                        }
                    }
                    break;

                case CONFIG_BTN_CANCELAR:
                    ReproducirSonido(Snd_Click_boton, 100, 0, 0);
                    irAEscena("menu");
                    break;

                case CONFIG_BTN_JUEGO_A:
                    ReproducirSonido(Snd_Click_boton, 100, 0, 0);
                    modo_juego = Jugar::MODO_JUEGO_VIDAS;
                    botones[CONFIG_BTN_JUEGO_A]->estaSeleccionado(true);
                    botones[CONFIG_BTN_JUEGO_B]->estaSeleccionado(false);
                    break;

                case CONFIG_BTN_JUEGO_B:
                    ReproducirSonido(Snd_Click_boton, 100, 0, 0);
                    modo_juego = Jugar::MODO_JUEGO_BASE;
                    botones[CONFIG_BTN_JUEGO_B]->estaSeleccionado(true);
                    botones[CONFIG_BTN_JUEGO_A]->estaSeleccionado(false);
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

/**
 * Crear y enviar paquete con la configuración de la partida
 */
void ConfigurarPartida::enviarConfiguracion() {
    size_t num_bytes;

    num_bytes = paquete.nuevoPqtConfiguracion(
        buffer,
        nombre_host.c_str(),
        ((modo_juego == Jugar::MODO_JUEGO_BASE) ? 0 : options_vidas[opt_vidas]),
        ((mapa_info->en_juego) ? mapa_info->id : -1)
    );

    cout << "[Protocolo] Enviando configuración" << endl;
    Net_enviar(buffer, num_bytes);
}


/**
 * Envia el mapa seleccionado al oponente
 */
void ConfigurarPartida::enviarMapa() {
    Uint64 tam_archivo;
    SDL_RWops *archivo = SDL_RWFromFile(mapa_info->ruta, "rb");

    if (archivo) {
        tam_archivo = SDL_RWsize(archivo);

        SDL_RWread(archivo, buffer, 1, tam_archivo);
        cout << "[Protocolo] Enviando mapa" << endl;
        Net_enviar(buffer, tam_archivo);

        SDL_RWclose(archivo);
    }
}
