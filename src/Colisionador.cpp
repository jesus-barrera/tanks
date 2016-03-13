#include "../include/Colisionador.h"

Colisionador::Colisionador() {
	this->area = NULL;
}

void Colisionador::fijarAreaColision(SDL_Rect *area) {
	this->area = area;
}

SDL_Rect *Colisionador::obtenerAreaColision() {
	return this->area;
}

void Colisionador::agregarColisionador(Colisionador *obj) {
	this->colisionadores.push_back(obj);
}

bool Colisionador::comprobarColision(SDL_Rect *this_area) {
	vector<Colisionador *>::iterator it;
	SDL_Rect *obj_area;
	bool en_colision;

	if (this_area == NULL) {
		this_area = this->area;
	}

	en_colision = false;

	for (it = this->colisionadores.begin(); it != this->colisionadores.end(); ++it) {
		obj_area = (*it)->obtenerAreaColision();

		if (SDL_HasIntersection(this_area, obj_area)) {
			this->enColision(*it);

			en_colision = true;
		}
	}

	return en_colision;
}

bool Colisionador::tieneEtiqueta(string etiqueta) {
	return this->etiqueta == etiqueta;
}