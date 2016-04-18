#include "../include/Jugar.h"

Tanque *Jugar::jugador;
Tanque *Jugar::tanque_j1;
Tanque *Jugar::tanque_j2;
Base *Jugar::base_1;
Base *Jugar::base_2;

void Jugar::entrar() {
	Editor::cargarMapa(MAPAS_RUTA"/campo_abierto.map", tanque_j1, base_1, tanque_j2, base_2);
	jugador = tanque_j2;
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
}

void Jugar::manejarEvento(SDL_Event &evento) {
	jugador->manejarEvento(evento);
}

void Jugar::inicializar() {
	tanque_j1 = new Tanque(TQ_TIPO_ROJO);
	tanque_j2 = new Tanque(TQ_TIPO_AZUL);

	base_1 = new Base();
	base_2 = new Base();
}

void Jugar::liberar() {
	delete(tanque_j1);
	delete(tanque_j2);
	delete(base_1);
	delete(base_2);
}
