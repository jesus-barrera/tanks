#include "../include/Jugar.h"

Tanque *Jugar::jugador;
Tanque *Jugar::tanque_j1;
Tanque *Jugar::tanque_j2;
Base *Jugar::base_1;
Base *Jugar::base_2;
Boton *Jugar::boton_salir;


void Jugar::inicializar() {
	tanque_j1 = new Tanque(TQ_TIPO_ROJO);
	tanque_j2 = new Tanque(TQ_TIPO_AZUL);

	base_1 = new Base(BASE_TIPO_ROJO);
	base_2 = new Base(BASE_TIPO_AZUL);

	tanque_j1->bala[0].agregarColisionador(tanque_j2);
	tanque_j1->bala[1].agregarColisionador(tanque_j2);
	tanque_j1->bala[2].agregarColisionador(tanque_j2);

	tanque_j1->bala[0].agregarColisionador(base_2);
	tanque_j1->bala[1].agregarColisionador(base_2);
	tanque_j1->bala[2].agregarColisionador(base_2);

	boton_salir = new Boton("Salir", 15, VENTANA_ALTO - 50);
	boton_salir->setViewport(&vista_estatus);
}

void Jugar::liberar() {
	delete(tanque_j1);
	delete(tanque_j2);
	delete(base_1);
	delete(base_2);
	delete(boton_salir);
}

void Jugar::entrar() {
	Editor::cargarMapa(MAPAS_RUTA"/campo_abierto.map", tanque_j1, base_1, tanque_j2, base_2);

	base_1->estaDestruido(false);
	base_2->estaDestruido(false);

	jugador = tanque_j1;
}

void Jugar::actualizar() {
	// Actualizar objetos
	tanque_j1->actualizar();
	tanque_j2->actualizar();

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
	
	base_1->renderizar();
	base_2->renderizar();

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
