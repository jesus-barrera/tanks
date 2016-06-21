#include "../include/globales.h"
#include "../include/Objeto.h"

Objeto::Objeto(int x, int y, direccion_t direccion, float velocidad, SDL_Texture *textura) {
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

float Objeto::obtenerVelocidad() {
	return this->velocidad;
}

SDL_Texture *Objeto::obtenerTextura() {
	return this->textura;
}

int Objeto::obtenerAlto() {
	return this->rect.h;
}

int Objeto::obtenerAncho() {
	return this->rect.w;
}

void Objeto::fijarPosicion(int x, int y) {
	this->pos_x = x;
	this->pos_y = y;
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

void Objeto::fijarVelocidad(float velocidad) {
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

void Objeto::renderizar(SDL_Rect *clip) {
	SDL_RenderCopyEx(renderer_principal, this->textura, clip, &(this->rect), angulo, NULL, SDL_FLIP_NONE);
}
