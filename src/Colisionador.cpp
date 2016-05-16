#include <cstring>
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

SDL_Rect *Colisionador::comprobarColision(SDL_Rect *this_area, SDL_Rect *colision_area) {
	vector<Colisionador *>::iterator it;
	SDL_Rect *obj_area;
	SDL_Rect *result;

	if (this_area == NULL) {
		this_area = this->area;
	}

	result = NULL;

	for (it = this->colisionadores.begin(); it != this->colisionadores.end(); ++it) {
		obj_area = (*it)->obtenerAreaColision();

		if (SDL_HasIntersection(this_area, obj_area)) {
			this->enColision(*it);

			memcpy(colision_area, obj_area, sizeof(*obj_area));
			result = colision_area;
		}
	}

	return result;
}

bool Colisionador::tieneEtiqueta(string etiqueta) {
	return this->etiqueta == etiqueta;
}