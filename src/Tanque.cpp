#include "../include/Tanque.h";

SDL_Texture *mover_sprites[TQ_NUM_FRAMES_MOVER];

void Tanque::actualizarSprite() {
	textura = mover_sprites[frame_num];
	SDL_QueryTexture(textura, NULL, NULL, &rect.w, &rect.h);
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
}

Tanque::~Tanque() {

}

bool Tanque::cargarMedios() {
	SDL_Texture *textura;	
	char nombre_archivo[50];

	for (int i = 0; i < TQ_NUM_FRAMES_MOVER; i++) {
		sprintf(nombre_archivo, "%s/mover_%d.png", TQ_RUTA_MEDIOS, i + 1);

		textura = cargarTextura(nombre_archivo);

		if (textura == NULL) {
			return false;
		} else {
			mover_sprites[i] = textura;
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
	SDL_Rect sig_rect = rect;

	switch (direccion) {
		case ARRIBA: sig_rect.y -= velocidad;
			break;
		case ABAJO: sig_rect.y += velocidad;
			break;
		case DERECHA: sig_rect.x += velocidad;
			break;
		case IZQUIERDA: sig_rect.x -= velocidad;
			break;
		default: ;
	}

	if ((sig_rect.x >= 0 && sig_rect.x < (vista_juego.w - rect.w)) && 
		(sig_rect.y >= 0 && sig_rect.y < (vista_juego.h - rect.h)) &&
		!Escenario::enColisionConMapa(sig_rect)) {
		
		rect = sig_rect;
	} else {
		fijarVelocidad(0);
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
			case SDLK_r:
				Escenario::crearMapaAleatorio();
				mover = false;
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

SDL_Rect Tanque::obtenerRect() {
	return rect;
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
		case IZQUIERDA: angulo = 270;
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
	SDL_RenderCopyEx(renderer_principal, textura, NULL, &rect, angulo, NULL, SDL_FLIP_NONE);
}