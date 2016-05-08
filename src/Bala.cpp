#include "../include/utiles.h"
#include "../include/Escenario.h"
#include "../include/Base.h"
#include "../include/Tanque.h"
#include "../include/Bala.h"

SDL_Texture *Bala::mover_sprites[BALA_NUM_FRAMES_MOVER];

Bala::Bala() {
	this->rect.h = this->rect.w = TAMANO_BLOQUE * 0.5;

	this->fijarAreaColision(&this->rect);
	this->etiqueta = BALA_ETIQUETA;

	frame_num = 0;
	actualizarSprite();
	animar_temp.iniciar();
}

void Bala::actualizarSprite() {
	this->textura = mover_sprites[0];
}

void Bala::sigFrame() {
	frame_num = (frame_num + 1) % BALA_NUM_FRAMES_MOVER;
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
	for (int i = 0; i < BALA_NUM_FRAMES_MOVER; i++) {
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

	if (velocidad != 0) {
        SDL_Rect sig_rect;
        SDL_Rect colision_area;
        SDL_Rect *colision;
        vector<SDL_Point> bloques;
        vector<SDL_Point>::iterator it;

        // Calcular desplazamiento
        double r = angulo * PI/180;
        double d = (velocidad * time_step);

        pos_x += sin(r) * d;
        pos_y -= cos(r) * d;

        /* Detección de colisiones */

        // Colisiones con el borde de la pantalla
        if (pos_x < 0) {
            pos_x = 0;
            choque = true;
        } else if (pos_x > (vista_juego.w - rect.w)) {
            pos_x = vista_juego.w - rect.w;
            choque = true;
        }

        if (pos_y < 0) {
            pos_y = 0;
            choque = true;
        } else if (pos_y > (vista_juego.h - rect.h)) {
            pos_y = vista_juego.h - rect.h;
            choque = true;
        }

        // Colisiones con objetos y bloques
        sig_rect = rect;

        sig_rect.x = (int)pos_x;
        sig_rect.y = (int)pos_y;

        bloques = Escenario::obtenerBloquesEnColision(sig_rect);

        if ((colision = comprobarColision(&sig_rect, &colision_area)) || bloques.size() > 0) {
            for (it = bloques.begin(); it != bloques.end(); ++it) {
                if (Escenario::obtenerBloque(*it) != BLOQUE_AGUA_1 && 
                    Escenario::obtenerBloque(*it) != BLOQUE_AGUA_2) {
                    
                    Escenario::destruirBloque((*it));

                    colision_area.x = (*it).x * TAMANO_BLOQUE;
                    colision_area.y = (*it).y * TAMANO_BLOQUE;

                    colision_area.w = TAMANO_BLOQUE;
                    colision_area.h = TAMANO_BLOQUE;

                    colision = &colision_area;
        
                    choque = true;
                }
            }

            if (colision) {    
                choque = true;

                // Truncar movimiento
                switch (direccion) {
                    case ARRIBA: pos_y = colision->y + colision->h;
                        break;
                    case ABAJO: pos_y = colision->y - rect.h;
                        break;
                    case DERECHA: pos_x = colision->x - rect.w;
                        break;
                    case IZQUIERDA: pos_x = colision->x + colision->w;
                        break;
                    default: ;
                }
            }
        }

        rect.x = (int)pos_x;
        rect.y = (int)pos_y;
	}

    if (choque) {
        disponible = true;
        fijarVelocidad(0);
    }

	return choque;
}

void Bala::animar() {
	if (animar_temp.obtenerTiempo() > (1000 / BALA_FRAMES_POR_SEC)) {
		if (velocidad != 0) {
			//sigFrame();
		}
		animar_temp.iniciar();
	}
}

void Bala::enColision(Colisionador *objeto) {
	if (objeto->tieneEtiqueta(BASE_ETIQUETA)) {
		((Base *) objeto)->estaDestruido(true);
	} else if (objeto->tieneEtiqueta(TQ_ETIQUETA)) {
		((Tanque *) objeto)->destruir();
	}
}

void Bala::Disparar(direccion_t direccion, int x, int y){
    fijarDireccion(direccion);
    fijarPosicion(x, y);
    fijarVelocidad(300);
    disponible = false;
}

void Bala::renderizar(){
    if(velocidad>0){
        Objeto::renderizar();
    }
}
