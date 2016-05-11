#include "../include/utiles.h"
#include "../include/MapaInfoDAO.h"
#include "../include/Editor.h"
#include "../include/Jugar.h"
#include "../include/network.h"

void renderizarTanque(Tanque *tanque) {
    tanque->renderizar();
    tanque->bala[0].renderizar();
    tanque->bala[1].renderizar();
    tanque->bala[2].renderizar();
}

Jugar::Jugar() {
    Tanque *t1, *t2;
    Base *b1, *b2;

    // Crear objetos
    jugador_1.tanque = new Tanque(TQ_TIPO_ROJO);
    jugador_2.tanque = new Tanque(TQ_TIPO_AZUL);

    jugador_1.base = new Base(BASE_TIPO_ROJO);
    jugador_2.base = new Base(BASE_TIPO_AZUL);

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

    mensaje = new Etiqueta("", VENTANA_PADDING, VENTANA_PADDING);

    nombre_jugador = new Etiqueta("", VENTANA_PADDING, VENTANA_PADDING);
    vidas_jugador = new Etiqueta("", VENTANA_PADDING, 45, DEFAULT_FONT_SIZE, COLOR_GRIS);
    
    nombre_oponente = new Etiqueta("", VENTANA_PADDING, 100);
    vidas_oponente = new Etiqueta("", VENTANA_PADDING, 130, DEFAULT_FONT_SIZE, COLOR_GRIS);

    // Crear elementos de UI
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
    if (modo_net == MODO_SERVIDOR) {
        jugador = &jugador_1;
        oponente = &jugador_2;
    } else {
        jugador = &jugador_2;
        oponente = &jugador_1;
    }

    jugador_1.base->estaDestruido(false);
    jugador_2.base->estaDestruido(false);

    if (modo_juego == MODO_JUEGO_VIDAS) {
        jugador_1.num_vidas = num_vidas;
        jugador_2.num_vidas = num_vidas;

        vidas_jugador->fijarTexto(to_string(num_vidas));
        vidas_oponente->fijarTexto(to_string(num_vidas));
    }

    estado = ST_JUGAR;

    enviado_temp.iniciar();
    recibido_temp.iniciar();
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
                cout << "[Debug] Evento: ";
                cout << "x: " << paquete.pos_x << ", ";
                cout << "y: " << paquete.pos_y << ", ";
                cout << "evento: " << (int)paquete.evento << ", ";
                cout << "velocidad: " << paquete.velocidad << endl;
                oponente->tanque->fijarPosicion(paquete.pos_x, paquete.pos_y);
                
                if (paquete.evento < 4) {
                    oponente->tanque->fijarDireccion((direccion_t)paquete.evento);
                } else if (paquete.evento == 4) {
                    oponente->tanque->disparar();
                }

                oponente->tanque->fijarVelocidad(paquete.velocidad);

            } else if (paquete.tipo == PQT_MANTENER_CONEXION) {
                cout << "[Debug] Mantener conexion" << endl; 
            } else {
                cout << "[Debug] Paquete descartado" << endl; 
            }
        } else if (recibido_temp.obtenerTiempo() > TIEMPO_CONEXION_PERDIDA) {
            mensaje->fijarTexto("Se perdio la conexion!");

            estado = ST_ERROR;
        }

        // Si no hay eventos, mantener conexion
        if (enviado_temp.obtenerTiempo() > TIEMPO_ESPERA) {
            int num_bytes = paquete.nuevoPqtMantenerConexion(buffer, "STILL ALIVE");
            Net_enviar(buffer, num_bytes);

            cout << "Mantener se envio" << endl;
            enviado_temp.iniciar();
        }
    
        // Actualizar objetos
        jugador_1.tanque->actualizar();
        jugador_2.tanque->actualizar();
    }
}

void Jugar::modoServidorActualizar() {

}

void Jugar::modoClienteActualizar() {

}

void Jugar::renderizar() {
    SDL_RenderSetViewport(renderer_principal, &vista_juego);
    
    // Renderizar fondo y bloques
    Escenario::renderizar();

    
    if (estado == ST_JUGAR) {
        // Renderizar objetos
        renderizarTanque(jugador_1.tanque);
        renderizarTanque(jugador_2.tanque);
        
        jugador_1.base->renderizar();
        jugador_2.base->renderizar();

        SDL_RenderSetViewport(renderer_principal, &vista_estatus);
        nombre_jugador->renderizar();
        nombre_oponente->renderizar();
        
        vidas_jugador->renderizar();
        vidas_oponente->renderizar();
        
        boton_salir->renderizar();
    } else if (estado == ST_ERROR) {
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
        
        if (jugador->tanque->manejarEvento(evento, buffer, &num_bytes)) {
            // Enviar evento
            Net_enviar(buffer, num_bytes);
            enviado_temp.iniciar();
        }
    }
}

void Jugar::abandonarPartida() {
    int num_bytes;

    // Despedir oponente
    num_bytes = paquete.nuevoPqtAbandonar(buffer, "BYE");
    Net_enviar(buffer, num_bytes);

    Net_terminar();
    irAEscena("menu");   
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