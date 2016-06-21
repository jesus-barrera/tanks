#include "../include/utiles.h"
#include "../include/MapaInfoDAO.h"
#include "../include/Editor.h"
#include "../include/Jugar.h"
#include "../include/Musica.h"
#include "../include/network.h"

Jugar::Jugar() {
    Tanque *t1, *t2;
    Base *b1, *b2;

    // Crear objetos
    jugador_1.tanque = new Tanque(JUGADOR_1);
    jugador_2.tanque = new Tanque(JUGADOR_2);

    jugador_1.base = new Base(JUGADOR_1);
    jugador_2.base = new Base(JUGADOR_2);

    // Definir colisiones entre los objetos
    t1 = jugador_1.tanque;
    t2 = jugador_2.tanque;

    b1 = jugador_1.base;
    b2 = jugador_2.base;

    for (int i = 0; i < MAX_BALAS; i++) {
        t1->bala[i].agregarColisionador(t2);
        t1->bala[i].agregarColisionador(b2);
        t1->bala[i].agregarColisionador(b1);

        t2->bala[i].agregarColisionador(t1);
        t2->bala[i].agregarColisionador(b1);
        t2->bala[i].agregarColisionador(b2);

        t1->agregarColisionador(t2);
        t2->agregarColisionador(t1);

        t1->agregarColisionador(b1);
        t1->agregarColisionador(b2);
        t2->agregarColisionador(b1);
        t2->agregarColisionador(b2);
    }

    jugador_1.tanque->fijarControles(Tanque::control_config[0]);
    jugador_2.tanque->fijarControles(Tanque::control_config[0]);

    /**
     * Crear elementos de UI
     */
    mensaje = new Etiqueta("", 15, 15);

    nombre_jugador = new Etiqueta("", 15, 15);
    vidas_jugador = new Etiqueta("", 15, 45, DEFAULT_FONT_SIZE, COLOR_GRIS);

    nombre_oponente = new Etiqueta("", 15, 100);
    vidas_oponente = new Etiqueta("", 15, 130, DEFAULT_FONT_SIZE, COLOR_GRIS);

    boton_salir = new Boton("Abandonar", 15, VENTANA_ALTO - 35);
    boton_salir->setViewport(&vista_estatus);
}

Jugar::~Jugar() {
    delete(jugador_1.tanque);
    delete(jugador_2.tanque);
    delete(jugador_1.base);
    delete(jugador_2.base);
    delete(boton_salir);
    delete(nombre_jugador);
    delete(nombre_oponente);
    delete(vidas_jugador);
    delete(vidas_oponente);
}

void Jugar::entrar() {
    cambiarMusicaFondo(MusicaFondoJugar);
    ReproducirMusicaFondo();

    if (modo_net == MODO_SERVIDOR) {
        jugador  = &jugador_1;
        oponente = &jugador_2;

        jugador_1.tanque->habilitarBalasDestruccion(true);
        jugador_2.tanque->habilitarBalasDestruccion(true);
    } else {
        jugador  = &jugador_2;
        oponente = &jugador_1;

        jugador_1.tanque->habilitarBalasDestruccion(false);
        jugador_2.tanque->habilitarBalasDestruccion(false);
    }

    jugador_1.base->estaDestruido(false);
    jugador_2.base->estaDestruido(false);

    if (modo_juego == MODO_JUEGO_VIDAS) {
        jugador_1.tanque->fijarNumVidas(num_vidas);
        jugador_2.tanque->fijarNumVidas(num_vidas);

        vidas_jugador->fijarTexto(inttostr(num_vidas));
        vidas_oponente->fijarTexto(inttostr(num_vidas));
    }

    jugador_1.tanque->capturarEstado();
    jugador_2.tanque->capturarEstado();

    ganador = 0;

    enviado_temp.iniciar();
    recibido_temp.iniciar();

    estado = ST_JUGAR;
}

void Jugar::actualizar() {
    if (estado == ST_JUGAR) {
        if (Net_recibir(buffer, TAM_BUFFER)) {
            recibido_temp.iniciar();

            paquete.analizar(buffer);

            if (paquete.tipo == PQT_ABANDONAR) {
                Net_terminar();

                mensaje->fijarTexto("Jugador \"" + nombre_oponente->obtenerTexto() + "\" abandono la partida");
                estado = ST_ERROR;

            } else if (paquete.tipo == PQT_EVENTO) {
                oponente->tanque->fijarPosicion(paquete.pos_x, paquete.pos_y);

                if (paquete.evento < EVENTO_DISPARO) {
                    oponente->tanque->fijarDireccion((direccion_t)paquete.evento);
                } else if (paquete.evento == EVENTO_DISPARO) {
                    oponente->tanque->disparar();
                }

                oponente->tanque->fijarVelocidad(paquete.velocidad);

            } else if (paquete.tipo == PQT_MANTENER_CONEXION) {
                /**
                 * El otro jugador sigue conectado
                 */
            } else {
                if (modo_net == MODO_CLIENTE) {
                    if (paquete.tipo == PQT_TERMINAR_PARTIDA) {
                        if (paquete.ganador == JUGADOR_2) {
                            mensaje->fijarTexto(GANADOR_MENSAJE);
                        } else {
                            mensaje->fijarTexto(PERDEDOR_MENSAJE);
                        }

                        estado = ST_FIN_PARTIDA;

                    } else  if (paquete.tipo == PQT_DESTRUIR_BLOQUE) {
                        SDL_Point bloque;

                        bloque.x = paquete.pos_x;
                        bloque.y = paquete.pos_y;
                        Escenario::destruirBloque(bloque);

                        if (paquete.pos_x2 != -1) {
                            bloque.x = paquete.pos_x2;
                            bloque.y = paquete.pos_y2;
                            Escenario::destruirBloque(bloque);
                        }

                        destruirBala(paquete.id_bala);

                    } else if (paquete.tipo == PQT_DESTRUIR_OBJETO) {
                        if (paquete.tipo_objeto == TIPO_OBJ_TANQUE) {
                            if (paquete.num_jugador == JUGADOR_1) {
                                jugador_1.tanque->destruir();
                            } else if (paquete.num_jugador == JUGADOR_2) {
                                jugador_2.tanque->destruir();
                            }
                        } else  if (paquete.tipo_objeto == TIPO_OBJ_BASE) {
                            if (paquete.num_jugador == JUGADOR_1) {
                                jugador_1.base->estaDestruido(true);
                            } else if (paquete.num_jugador == JUGADOR_2) {
                                jugador_2.base->estaDestruido(true);
                            }
                        }

                        destruirBala(paquete.id_bala);
                    }
                }
            }
        } else if (recibido_temp.obtenerTiempo() > TIEMPO_CONEXION_PERDIDA) {
            mensaje->fijarTexto("Se perdio la conexion!");

            estado = ST_ERROR;
        }

        // Si el usuario esta inactivo, enviar mensaje para mantener viva la conexión
        if (enviado_temp.obtenerTiempo() > TIEMPO_ESPERA) mantenerConexion();

        // Actualizar objetos
        jugador_1.tanque->actualizar();
        jugador_2.tanque->actualizar();

        // Actualizar contador
        if (modo_juego == MODO_JUEGO_VIDAS)  {
            actualizarContadorVidas(jugador, vidas_jugador);
            actualizarContadorVidas(oponente, vidas_oponente);
        }

        // Determinar si hay un ganador
        if (modo_net == MODO_SERVIDOR) comprobarGanador();
    }
}

void Jugar::renderizar() {
    SDL_RenderSetViewport(renderer_principal, &vista_juego);

    // Renderizar fondo y bloques
    Escenario::renderizar();

    if (estado == ST_JUGAR) {
        // Renderizar objetos
        jugador_1.tanque->renderizar();
        jugador_2.tanque->renderizar();

        jugador_1.base->renderizar();
        jugador_2.base->renderizar();

        SDL_RenderSetViewport(renderer_principal, &vista_estatus);
        nombre_jugador->renderizar();
        nombre_oponente->renderizar();

        if (modo_juego == MODO_JUEGO_VIDAS) {
            vidas_jugador->renderizar();
            vidas_oponente->renderizar();
        }

        boton_salir->renderizar();

    } else if (estado == ST_ERROR || estado == ST_FIN_PARTIDA) {
        DetenerCanal(jugador_1.tanque->tipo+1);
        DetenerCanal(jugador_2.tanque->tipo+1);
        renderizarCapaGris();
        mensaje->renderizar();

        SDL_RenderSetViewport(renderer_principal, &vista_estatus);
        boton_salir->renderizar();
    }
}

void Jugar::manejarEvento(SDL_Event &evento) {
    int num_bytes;

    if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT) {
        if (boton_salir->isMouseOver()) {
            abandonarPartida();
        }
    } else if (estado == ST_JUGAR) {
        if (jugador->tanque->manejarEvento(evento)) {
            enviado_temp.iniciar();
        }
    }
}

void Jugar::mantenerConexion() {
    int num_bytes;

    num_bytes = paquete.nuevoPqtMantenerConexion(buffer, "STILL ALIVE");

    Net_enviar(buffer, num_bytes);

    enviado_temp.iniciar();
}

void Jugar::comprobarGanador() {
    if (modo_net == MODO_SERVIDOR) {
        if (modo_juego == MODO_JUEGO_VIDAS) {
            if (jugador_2.tanque->obtenerNumVidas() == 0) {
                ganador = JUGADOR_1;
            } else if (jugador_1.tanque->obtenerNumVidas() == 0) {
                ganador = JUGADOR_2;
            }
        } else if (modo_juego == MODO_JUEGO_BASE) {
            if (jugador_2.base->estaDestruido()) {
                ganador = JUGADOR_1;
            } else if (jugador_1.base->estaDestruido()) {
                ganador = JUGADOR_2;
            }
        }

        if (ganador) {
            int num_bytes = paquete.nuevoPqtTerminarPartida(buffer, ganador);
            Net_enviar(buffer, num_bytes);

            if (ganador == 1) {
                mensaje->fijarTexto(GANADOR_MENSAJE);
            } else {
                mensaje->fijarTexto(PERDEDOR_MENSAJE);
            }

            estado = ST_FIN_PARTIDA;
        }
    }
}

void Jugar::abandonarPartida() {
    int num_bytes;

    num_bytes = paquete.nuevoPqtAbandonar(buffer, "BYE");
    Net_enviar(buffer, num_bytes);
    Net_terminar();

    if (estado == ST_FIN_PARTIDA) {
        if (modo_net == MODO_SERVIDOR) {
            irAEscena("nueva-partida");
        } else {
            irAEscena("conectar");
        }
    } else {
        irAEscena("menu");
    }
}

void Jugar::actualizarContadorVidas(Jugador *jugador, Etiqueta *contador) {
    int num_vidas;

    num_vidas = jugador->tanque->obtenerNumVidas();

    if (num_vidas != jugador->num_vidas) {
        jugador->num_vidas = num_vidas;

        contador->fijarTexto(inttostr(num_vidas));
    }
}

void Jugar::destruirBala(Uint8 id_bala) {
    if ((id_bala >> 4) == JUGADOR_1) {
        int num_bala = id_bala & 0x0F;

        jugador_1.tanque->bala[num_bala].fijarVelocidad(0);   
        jugador_1.tanque->bala[num_bala].disponible = true;   
    }
}

bool Jugar::cargarMapaPorId(Uint32 id) {
    MapaInfoDAO mapas;
    MapaInfo info;

    mapas.fijarArchivo(GAME_MAPS_INFO);

    if (mapas.obtener(id, &info)) {
        Editor::cargarMapa(info.ruta, jugador_1.tanque, jugador_1.base, jugador_2.tanque, jugador_2.base);
        return true;
    } else {
        return false;
    }
}

bool Jugar::cargarMapaPorRuta(const char *ruta) {
    Editor::cargarMapa(ruta, jugador_1.tanque, jugador_1.base, jugador_2.tanque, jugador_2.base);
    return true;
}

void Jugar::fijarModoJuego(int modo) {
    modo_juego = modo;
}

void Jugar::fijarNumVidas(int vidas) {
    num_vidas = vidas;
}

void Jugar::fijarModoNet(int modo) {
    modo_net = modo;
}

void Jugar::fijarNombreJugador(string nombre) {
    nombre_jugador->fijarTexto(nombre);
}

void Jugar::fijarNombreOponente(string nombre) {
    nombre_oponente->fijarTexto(nombre);
}
