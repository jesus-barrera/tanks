#include "../include/Objeto.h"

Objeto::Objeto(int x, int y, direccion_t direccion, int velocidad, SDL_Texture *textura) {
	fijarPosicion(x, y);
	fijarDireccion(direccion);
	fijarVelocidad(velocidad);
	fijarTextura(textura);
}

SDL_Rect Objeto::obtenerRect() {
	return this->rect;
}

SDL_Point Objeto::obtenerPosicion() {
	return {this->rect.x, this->rect.y};
}

direccion_t Objeto::obtenerDireccion() {
	return this->direccion;
}

int Objeto::obtenerVelocidad() {
	return this->velocidad;
}

SDL_Texture *Objeto::obtenerTextura() {
	return this->textura;
}

void Objeto::fijarPosicion(int x, int y) {
	this->rect.x = x;
	this->rect.y = y;
}

void Objeto::fijarDireccion(direccion_t direccion) {
	switch (direccion) {
		case ARRIBA: this->angulo = 0;
			break;
		case ABAJO: this->angulo = 180;
			break;
		case DERECHA: this->angulo = 90;
			break;
		case IZQUIERDA: this->angulo = 270;
			break;
		default: 
			return;
	}
	
	this->direccion = direccion;
}

void Objeto::fijarVelocidad(int velocidad) {
	this->velocidad = velocidad;
}

void Objeto::fijarTextura(SDL_Texture *textura) {
	this->textura = textura;
}

void Objeto::rotar(direccion_t direccion) {
	int dir = this->direccion;

	if (direccion == ARRIBA || direccion == DERECHA) {
		dir = (dir + 1) % TOTAL_DIRECCIONES; 

	} else if (direccion == ABAJO || direccion == IZQUIERDA) {
		if (--dir < 0) {
			dir = (int)IZQUIERDA;
		}
	}
	
	this->fijarDireccion((direccion_t)dir);	
}

void Objeto::agregarColisionador(Objeto *objeto) {
	this->colisionadores.push_back(objeto);
}

bool Objeto::comprobarColision(SDL_Rect *rect) {
	vector<Objeto *>::iterator it;
	SDL_Rect colision_rect;

	if (rect == NULL) {
		rect = &this->rect;
	}

	for (it = this->colisionadores.begin(); it != this->colisionadores.end(); ++it) {
		colision_rect = (*it)->obtenerRect();

		if (SDL_HasIntersection(rect, &colision_rect)) {
			return true;
		} // else continuar
	}

	return false;
}

void Objeto::renderizar(SDL_Rect *clip) {
	SDL_RenderCopyEx(renderer_principal, this->textura, clip, &(this->rect), angulo, NULL, SDL_FLIP_NONE);
}