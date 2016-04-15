#include "../include/Bala.h";

SDL_Texture *Bala::mover_sprites[TQ_NUM_FRAMES_MOVER];

Bala::Bala() {
	this->rect.h = this->rect.w = TAMANO_BLOQUE;

	this->fijarAreaColision(&this->rect);
	this->etiqueta = TQ_ETIQUETA;

	frame_num = 0;
	actualizarSprite();
	animar_temp.iniciar();
}

void Bala::actualizarSprite() {
	this->textura = mover_sprites[0];
}

void Bala::sigFrame() {
	frame_num = (frame_num + 1) % TQ_NUM_FRAMES_MOVER;
	actualizarSprite();
}


bool Bala::inicializar() {
	SDL_Texture *text;

    text = cargarTextura("media/tank/bala_1.png");
    if (text == NULL) {
        mostrarError("Error al cargar imagen bala");
        return false;
    } else {
        mover_sprites[0] = text;
    }
	return true;
}

void Bala::liberarMemoria() {
	for (int i = 0; i < TQ_NUM_FRAMES_MOVER; i++) {
		SDL_DestroyTexture(mover_sprites[i]);
	}
}

void Bala::actualizar() {
    mover();
    //cout<<"direccion de memoria: "<<textura<<endl;
    //animar();
    //renderizar();
}

bool Bala::mover() {
    bool choque=false;
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
            choque=false;
			rect = sig_rect;
		} else {
			fijarVelocidad(0);
			choque=true;
			disponible=true;
			for (it = bloques.begin(); it != bloques.end(); ++it) {
				Escenario::destruirBloque((*it));
				choque=true;
				disponible=true;
			}
		}
	}
	return choque;
}

void Bala::animar() {
	if (animar_temp.obtenerTiempo() > (1000 / TQ_FRAMES_POR_SEC)) {
		if (velocidad != 0) {
			//sigFrame();
		}
		animar_temp.iniciar();
	}
}

void Bala::enColision(Colisionador *objeto) {
	if (objeto->tieneEtiqueta(BASE_ETIQUETA)) {
		((Base *)objeto)->estaDestruido(true);
	}
}

void Bala::Disparar(direccion_t direccion, int x, int y){
    fijarDireccion(direccion);
    fijarPosicion(x, y);
    fijarVelocidad(5);
    disponible = false;
}

void Bala::renderizar(){
    if(velocidad>0){
        Objeto::renderizar();
    }
}
