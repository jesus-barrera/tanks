#include "../include/Juego.h"

Tanque *Juego::tanque_j1;
Tanque *Juego::tanque_j2;
Base *Juego::base_1;
Base *Juego::base_2;

void Juego::entrar() {

}

void Juego::actualizar() {
	tanque_j1->actualizar();
	tanque_j2->actualizar();

	SDL_RenderSetViewport(renderer_principal, &vista_juego);
	Escenario::renderizar();

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
}

void Juego::manejarEvento(SDL_Event &evento) {
	tanque_j1->manejarEvento(evento);
}

void Juego::inicializar() {
	tanque_j1 = new Tanque();
	tanque_j2 = new Tanque();

	base_1 = new Base();
	base_2 = new Base();
}

void Juego::liberar() {
	delete(tanque_j1);
	delete(tanque_j2);
	delete(base_1);
	delete(base_2);
}
