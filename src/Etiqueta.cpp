#include "../include/utiles.h"
#include "../include/globales.h"
#include "../include/Etiqueta.h"

Etiqueta::Etiqueta(string texto, int x, int y, int tam_fuente, SDL_Color color) {
	this->texto = texto;
	this->tam_fuente = tam_fuente;
	this->color = color;
	this->rect.x = x;
	this->rect.y = y;

	this->textura = NULL;
	this->actualizarTextura();
}

Etiqueta::~Etiqueta() {
	SDL_DestroyTexture(this->textura);
}

void Etiqueta::actualizarTextura() {
	SDL_DestroyTexture(this->textura);

	this->textura = renderizarTexto(this->texto, this->color, this->tam_fuente);

	SDL_QueryTexture(this->textura, NULL, NULL, &this->rect.w, &this->rect.h);
}

void Etiqueta::fijarTamFuente(int tam) {
	this->tam_fuente = tam;
	this->actualizarTextura();
}

void Etiqueta::fijarTexto(string texto) {
	this->texto = texto;
	this->actualizarTextura();
}

void Etiqueta::fijarColor(SDL_Color color) {
	this->color = color;
	this->actualizarTextura();
}

void Etiqueta::fijarPosicion(int x, int y) {
	this->rect.x = x;
	this->rect.y = y;
}

int Etiqueta::obtenerAncho() {
	return this->rect.w;
}

int Etiqueta::obtenerAltura() {
	return this->rect.h;
}

string Etiqueta::obtenerTexto() {
	return this->texto;
}

void Etiqueta::renderizar() {
	SDL_RenderCopy(renderer_principal, this->textura, NULL, &this->rect);
}
