#include "../include/Base.h"
#include "../include/Musica.h"

SDL_Texture *Base::sprite = NULL;
SDL_Rect Base::clips[BASE_NUM_CLIPS];

bool Base::inicializar() {
	Base::sprite = cargarTextura("media/textures/aguila.png");

	Base::clips[BASE_NORMAL_CLIP].w = 16;
	Base::clips[BASE_NORMAL_CLIP].h = 16;
	Base::clips[BASE_NORMAL_CLIP].x = 0;
	Base::clips[BASE_NORMAL_CLIP].y = 0;

	Base::clips[BASE_DESTRUIDA_CLIP].w = 16;
	Base::clips[BASE_DESTRUIDA_CLIP].h = 16;
	Base::clips[BASE_DESTRUIDA_CLIP].x = 16;
	Base::clips[BASE_DESTRUIDA_CLIP].y = 0;

	return Base::sprite != NULL;
}

void Base::liberarMemoria() {
	SDL_DestroyTexture(Base::sprite);
}

Base::Base(int tipo) {
	this->rect.w = this->rect.h = TAMANO_BLOQUE * 2;

	this->fijarTextura(Base::sprite);
	this->fijarAreaColision(&this->rect);
	this->etiqueta = BASE_ETIQUETA;

	this->tipo = tipo;

	this->estaDestruido(false);
}

bool Base::estaDestruido() {
	return this->esta_destruido;
}

void Base::estaDestruido(bool destruido) {
	this->esta_destruido = destruido;

	if (destruido) {
        ReproducirSonido(Snd_Explosion, 100, 0, 0);
		this->clip = Base::clips[BASE_DESTRUIDA_CLIP];
	} else {
		this->clip = Base::clips[BASE_NORMAL_CLIP];
	}
}

void Base::renderizar() {
	if (this->tipo == JUGADOR_1) {
		SDL_SetTextureColorMod(this->textura, 255, 100, 0);
	} else if (this->tipo == JUGADOR_2) {
		SDL_SetTextureColorMod(this->textura, 0, 70, 255);
	}

	Objeto::renderizar(&(this->clip));
}

void Base::enColision(Colisionador *objeto) {
	;
}
