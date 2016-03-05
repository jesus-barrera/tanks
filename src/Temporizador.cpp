#include "../include/Temporizador.h";

Temporizador::Temporizador() {
	this->detener();
}

void Temporizador::iniciar() {
	this->tiempo_inicio = SDL_GetTicks();
}

void Temporizador::detener() {
	this->tiempo_inicio = -1;
}

unsigned int Temporizador::obtenerTiempo() {
	if (this->tiempo_inicio != -1) {
		return SDL_GetTicks() - this->tiempo_inicio;
	} else {
		return 0;
	}
}