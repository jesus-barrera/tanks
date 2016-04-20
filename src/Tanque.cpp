#include "../include/Tanque.h";

SDL_Texture *Tanque::mover_sprite;
SDL_Texture *Tanque::explosion_sprite;
SDL_Rect Tanque::explosion_clips[];
SDL_Rect Tanque::mover_clips[];

Tanque::Tanque(int tipo) {
	this->rect.h = this->rect.w = TAMANO_BLOQUE * TQ_TAMANO * 0.85;

	this->fijarAreaColision(&this->rect);
	this->etiqueta = TQ_ETIQUETA;

	this->estado = TQ_ST_MOVER;
	this->tipo = tipo;

	frame_num = 0;
	animar_temp.iniciar();
}

bool Tanque::inicializar() {
	// Cargar textura para la explosión usando colorkey
	mover_sprite = cargarTextura(TQ_RUTA_MEDIOS"/mover.png");

	// Cargar textura para la explosión usando colorkey
	explosion_sprite = cargarTextura(TQ_RUTA_MEDIOS"/explosion.png", 0, 0xff, 0xff);

	// Definir los clips para la animación de explosión
	explosion_clips[0].x = 0;
	explosion_clips[0].y = 0;
	explosion_clips[0].w = 16;
	explosion_clips[0].h = 16;

	explosion_clips[1].x = 16;
	explosion_clips[1].y = 0;
	explosion_clips[1].w = 16;
	explosion_clips[1].h = 16;

	explosion_clips[2].x = 32;
	explosion_clips[2].y = 0;
	explosion_clips[2].w = 16;
	explosion_clips[2].h = 16;

	explosion_clips[3].x = 48;
	explosion_clips[3].y = 0;
	explosion_clips[3].w = 32;
	explosion_clips[3].h = 32;

	explosion_clips[4].x = 80;
	explosion_clips[4].y = 0;
	explosion_clips[4].w = 32;
	explosion_clips[4].h = 32;


	// Definir clips para la animación de mover
	for (int i = 0; i < TQ_NUM_FRAMES_MOVER; i++) {
		mover_clips[i].x = i * 25;
		mover_clips[i].y = 0;
		mover_clips[i].w = 25;
		mover_clips[i].h = 29;
	}

	return true;
}

void Tanque::liberarMemoria() {
	// Eliminar texturas
	SDL_DestroyTexture(mover_sprite);
	SDL_DestroyTexture(explosion_sprite);
}

bool Tanque::comprobarAnim() {
	return animar_temp.obtenerTiempo() >= (1000 / TQ_FRAMES_POR_SEC);
}

void Tanque::actualizar() {
	switch (estado) {
		case TQ_ST_MOVER:
			mover();

			if (velocidad > 0 && comprobarAnim()) {
				frame_num = (frame_num + 1) % TQ_NUM_FRAMES_MOVER;
				animar_temp.iniciar();
			}

			break;
		case TQ_ST_EXPLOTAR:
			if (comprobarAnim()) {
				if (++frame_num == TQ_NUM_FRAMES_EXPLOSION) {
					estado = TQ_ST_ESPERAR;

					reaparecer_temp.iniciar();
				} else {
					animar_temp.iniciar();
				}
			}
			break;
		case TQ_ST_ESPERAR:
			if (reaparecer_temp.obtenerTiempo() >= TQ_REAPARECER_TIEMPO) {
				estado = TQ_ST_MOVER;
				frame_num = 0;
			}
			break;
		default: ;
	}

	bala[0].actualizar();
	bala[1].actualizar();
	bala[2].actualizar();
}

void Tanque::mover() {
	if (velocidad != 0) {
		SDL_Rect sig_rect = rect;
		vector<SDL_Point> bloques;
		vector<SDL_Point>::iterator it;
		
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

void Tanque::manejarEvento(SDL_Event &evento) {
	static SDL_Keycode tecla_actual = -1;

	bool mover = true;
	SDL_Keycode tecla;

	if (estado == TQ_ST_MOVER && evento.type == SDL_KEYDOWN && evento.key.repeat == 0) {
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
            	int x, y;

            	x = rect.x;
            	y = rect.y;

            	if (direccion == IZQUIERDA 	|| direccion == DERECHA) {
            		y += rect.h / 2 - bala[0].obtenerAlto() / 2;
            	} else {
            		x += rect.w / 2 - bala[0].obtenerAncho() / 2;
            	}

                if(bala[0].disponible){
                    bala[0].Disparar(direccion, x, y);
                }else{
                    if(bala[1].disponible){
                        bala[1].Disparar(direccion, x, y);
                    }else{
                        if(bala[2].disponible){
                            bala[2].Disparar(direccion, x, y);
                        }
                    }
                }
                mover=false;
                break;
            case SDLK_e:
            	destruir();
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

void Tanque::enColision(Colisionador *objeto) {

}

void Tanque::renderizar() {
	switch (estado) {
		case TQ_ST_MOVER:
			textura = Tanque::mover_sprite;

			if (this->tipo == TQ_TIPO_ROJO) {
				SDL_SetTextureColorMod(textura, 255, 255, 255);
			} else {
				SDL_SetTextureColorMod(textura, 0, 200, 255);
			}

			Objeto::renderizar(&Tanque::mover_clips[frame_num]);
			break;
		case TQ_ST_EXPLOTAR: {
			SDL_Rect dst_rect;
			SDL_Rect *clip = &Tanque::explosion_clips[frame_num];

			textura = Tanque::explosion_sprite;

			dst_rect = this->rect;
			dst_rect.w = clip->w * 2; 
			dst_rect.h = clip->h * 2; 

			if (frame_num > 2) {
				dst_rect.x -= 16;
				dst_rect.y -= 16;
			}

			SDL_RenderCopy(renderer_principal, textura, clip, &dst_rect);
			break;
		}
		default: ;
	}
}

void Tanque::destruir() {
	estado = TQ_ST_EXPLOTAR;
	fijarVelocidad(0);

	frame_num = 0;
}