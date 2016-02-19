#include <stdio.h>
#include "../include/Tanque.h";

SDL_Texture *mover_sprites[TQ_NUM_FRAMES_MOVER];

void Tanque::actualizarSprite() {
	frame_actual = mover_sprites[frame_num];
	SDL_QueryTexture(frame_actual, NULL, NULL, &rect.w, &rect.h);

}

void Tanque::sigFrame() {
	frame_num = (frame_num + 1) % TQ_NUM_FRAMES_MOVER;
	actualizarSprite();
}

Tanque::Tanque(int x, int y, direccion_t direccion) {
	fijarDireccion(direccion);
	fijarPosicion(x, y);
	fijarVelocidad(0);
	frame_num = 0;
	actualizarSprite();

	tiempo_inicio = SDL_GetTicks();
}

Tanque::~Tanque() {

}

bool Tanque::cargarMedios() {
	SDL_Texture *textura;
	SDL_Surface *surface;
	char nombre_archivo[50];
	int i;

	for (i = 0; i < TQ_NUM_FRAMES_MOVER; i++) {
		sprintf(nombre_archivo, "%s/mover_%d.png", TQ_RUTA_MEDIOS, i + 1);

		surface = IMG_Load(nombre_archivo);

		if (surface == NULL) {
			return false;
		} else {
			textura = SDL_CreateTextureFromSurface(renderer_principal, surface);

			if (textura == NULL) {
				return false;
			} else {
				mover_sprites[i] = textura;
			}

			SDL_FreeSurface(surface);
		}
	}

	return true;
}

void Tanque::liberarMemoria() {
	for (int i = 0; i < TQ_NUM_FRAMES_MOVER; i++) {
		SDL_DestroyTexture(mover_sprites[i]);
	}
}

void Tanque::actualizar() {
	mover();
	animar();
}

void Tanque::mover() {
	int x, y;

	x = rect.x;
	y = rect.y;

	switch (direccion) {
		case ARRIBA: y -= velocidad;
			break;
		case ABAJO: y += velocidad;
			break;
		case DERECHA: x += velocidad;
			break;
		case IZQUIERDA: x -= velocidad;
			break;
		default: ;
	}

	if ((x >= 0 && x < (VENTANA_ANCHO * 0.8 - rect.w)) && (y >= 0 && y < (VENTANA_ALTO - rect.h))) {
		rect.x = x;
		rect.y = y;
	}
}

void Tanque::animar() {
	if ((SDL_GetTicks() - tiempo_inicio) > 1000 / TQ_FRAMES_POR_SEC) {
		if (velocidad != 0) {
			sigFrame();
		}

		tiempo_inicio = SDL_GetTicks();
	}
}

void Tanque::manejarEvento(SDL_Event &evento) {
	static SDL_Keycode tecla_actual = -1;

	bool mover = true;
	SDL_Keycode tecla;

	if (evento.type == SDL_KEYDOWN && evento.key.repeat == 0) {

		tecla = evento.key.keysym.sym;

		switch (tecla) {
			case SDLK_UP:
				fijarDireccion(ARRIBA);
				break;
			case SDLK_DOWN:
				fijarDireccion(ABAJO);
				break;
			case SDLK_LEFT:
				fijarDireccion(IZQUIERDA);
				break;
			case SDLK_RIGHT:
				fijarDireccion(DERECHA);
				break;
			default :
				mover = false;
		}

		if (mover) {
			tecla_actual = tecla;
			fijarVelocidad(2);
		}
	} else if (evento.type == SDL_KEYUP && evento.key.repeat == 0) {
		tecla = evento.key.keysym.sym;

		if (tecla == tecla_actual) {
			tecla_actual = -1;
			fijarVelocidad(0);
		}
	}

}

void Tanque::fijarPosicion(int x, int y) {
	rect.x = x;
	rect.y = y;
}

void Tanque::fijarDireccion(direccion_t direccion) {
	switch (direccion) {
		case ARRIBA: angulo = 0;
			break;
		case ABAJO: angulo = 180;
			break;
		case DERECHA: angulo = 90;
			break;
		case IZQUIERDA: angulo = -90;
			break;
		default: 
			return;
	}
	
	this->direccion = direccion;
}

void Tanque::fijarVelocidad(int velocidad) {
	this->velocidad = velocidad;
}

void Tanque::renderizar() {
	SDL_RenderCopyEx(renderer_principal, frame_actual, NULL, &rect, angulo, NULL, SDL_FLIP_NONE);
}