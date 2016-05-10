#include "../include/globales.h"
#include "../include/MapaInfoDAO.h"
#include "../include/Editor.h"
#include "../include/Jugar.h"

int Jugar::modo_juego;
int Jugar::num_vidas;

Tanque *Jugar::jugador;
Tanque *Jugar::tanque_j1;
Tanque *Jugar::tanque_j2;
Base *Jugar::base_j1;
Base *Jugar::base_j2;
Boton *Jugar::boton_salir;

void Jugar::inicializar() {
    // Crear objetos
    tanque_j1 = new Tanque(TQ_TIPO_ROJO);
    tanque_j2 = new Tanque(TQ_TIPO_AZUL);

    base_j1 = new Base(BASE_TIPO_ROJO);
    base_j2 = new Base(BASE_TIPO_AZUL);

    // Definir colisiones entre los objetos
    for (int i = 0; i < MAX_BALAS; i++) {
        tanque_j1->bala[i].agregarColisionador(tanque_j2);
        tanque_j1->bala[i].agregarColisionador(base_j2);
        tanque_j1->bala[i].agregarColisionador(base_j1);

        tanque_j2->bala[i].agregarColisionador(tanque_j1);
        tanque_j2->bala[i].agregarColisionador(base_j1);
        tanque_j2->bala[i].agregarColisionador(base_j2);

        tanque_j2->agregarColisionador(tanque_j1);
        tanque_j1->agregarColisionador(tanque_j2);
        
        tanque_j1->agregarColisionador(base_j1);
        tanque_j1->agregarColisionador(base_j2);
        tanque_j2->agregarColisionador(base_j1);
        tanque_j2->agregarColisionador(base_j2);
    }

    tanque_j1->fijarControles(Tanque::control_config[0]);
    tanque_j2->fijarControles(Tanque::control_config[1]);

    // Crear elementos de UI
    boton_salir = new Boton("Abandonar", 15, VENTANA_ALTO - 35);
    boton_salir->setViewport(&vista_estatus);

    jugador = tanque_j1;
}

void Jugar::liberarMemoria() {
    delete(tanque_j1);
    delete(tanque_j2);
    delete(base_j1);
    delete(base_j2);
    delete(boton_salir);
}

void Jugar::entrar() {
    base_j1->estaDestruido(false);
    base_j2->estaDestruido(false);
}

void Jugar::actualizar() {
    // Actualizar objetos
    tanque_j1->actualizar();
    tanque_j2->actualizar();
}

void Jugar::renderizar() {
    SDL_RenderSetViewport(renderer_principal, &vista_juego);
    
    // Renderizar fondo y bloques
    Escenario::renderizar();

    // Renderizar objetos
    tanque_j1->renderizar();
    tanque_j2->renderizar();
    
    tanque_j1->bala[0].renderizar();
    tanque_j1->bala[1].renderizar();
    tanque_j1->bala[2].renderizar();
    
    tanque_j2->bala[0].renderizar();
    tanque_j2->bala[1].renderizar();
    tanque_j2->bala[2].renderizar();
    
    base_j1->renderizar();
    base_j2->renderizar();

    SDL_RenderSetViewport(renderer_principal, &vista_estatus);
    boton_salir->renderizar();
}

void Jugar::manejarEvento(SDL_Event &evento) {
    if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT) {
        if (boton_salir->isMouseOver()) {
            irAEscena("menu");
        }
    } else {
        jugador->manejarEvento(evento);
    }
}

bool Jugar::cargarMapaPorId(Uint32 id) {
    MapaInfoDAO mapas;
    MapaInfo info;

    mapas.fijarArchivo(GAME_MAPS_INFO);
    
    if (mapas.obtener(id, &info)) {
        Editor::cargarMapa(info.ruta, tanque_j1, base_j1, tanque_j2, base_j2);
        return true;
    } else {
        return false;
    }
}

bool Jugar::cargarMapaPorRuta(const char *ruta) {
    Editor::cargarMapa(ruta, tanque_j1, base_j1, tanque_j2, base_j2);
    return true;
}

void Jugar::fijarModoJuego(int modo) {
    modo_juego = modo;
}

void Jugar::fijarNumVidas(int vidas) {
    num_vidas = vidas;
}