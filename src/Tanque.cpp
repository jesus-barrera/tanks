#include "../include/Tanque.h";

SDL_Texture *Tanque::mover_sprites[TQ_NUM_FRAMES_MOVER];

Tanque::Tanque() {
	this->rect.h = this->rect.w = TAMANO_BLOQUE * TQ_TAMANO * 0.85;

	this->fijarAreaColision(&this->rect);
	this->etiqueta = TQ_ETIQUETA;

	frame_num = 0;
	actualizarSprite();
	animar_temp.iniciar();
}

void Tanque::actualizarSprite() {
	this->textura = mover_sprites[frame_num];
}

void Tanque::sigFrame() {
	frame_num = (frame_num + 1) % TQ_NUM_FRAMES_MOVER;
	actualizarSprite();
}


bool Tanque::inicializar() {
	int w, h;
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
	bala[0].actualizar();
	bala[1].actualizar();
	bala[2].actualizar();
}

void Tanque::mover() {
	SDL_Rect sig_rect = rect;
	vector<SDL_Point> bloques;
	vector<SDL_Point>::iterator it;

	if (velocidad != 0) {
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

		bloques = Escenario::obtenerBloquesEnColision(sig_rect);

		if ((sig_rect.x >= 0 && sig_rect.x < (vista_juego.w - rect.w)) &&
			(sig_rect.y >= 0 && sig_rect.y < (vista_juego.h - rect.h)) &&
			!comprobarColision(&sig_rect) && bloques.size() == 0) {

			rect = sig_rect;
		} else {
			fijarVelocidad(0);
		}
	}
}

void Tanque::animar() {
	if (animar_temp.obtenerTiempo() > (1000 / TQ_FRAMES_POR_SEC)) {
		if (velocidad != 0) {
			sigFrame();
		}

		animar_temp.iniciar();
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
            case SDLK_q:
                if(bala[0].disponible){
                    bala[0].Disparar(direccion, rect.x, rect.y);
                }else{
                    if(bala[1].disponible){
                        bala[1].Disparar(direccion,rect.x,rect.y);
                    }else{
                        if(bala[2].disponible){
                            bala[2].Disparar(direccion,rect.x,rect.y);
                        }
                    }
                }
                mover=false;
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

//for(int disparos=0; disparos<balasdisparadas; disparos++){
//    bala[]
//}

void Tanque::enColision(Colisionador *objeto) {
	if (objeto->tieneEtiqueta(BASE_ETIQUETA)) {
		((Base *)objeto)->estaDestruido(true);
	}
}
