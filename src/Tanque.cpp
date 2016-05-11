#include <cmath>

#include "../include/globales.h"
#include "../include/utiles.h"
#include "../include/Paquete.h"

#include "../include/Tanque.h"

// Definir atributos estáticos
SDL_Texture *Tanque::mover_sprite;
SDL_Texture *Tanque::explosion_sprite;
SDL_Rect Tanque::explosion_clips[];
SDL_Rect Tanque::mover_clips[];
int Tanque::control_config[][NUM_ACCIONES];

Tanque::Tanque(int tipo) {
    this->rect.h = this->rect.w = TAMANO_BLOQUE * TQ_TAMANO * 0.85;

    this->fijarAreaColision(&this->rect);
    this->etiqueta = TQ_ETIQUETA;

    this->estado = TQ_ST_MOVER;
    this->tipo = tipo;

    this->tecla_actual = -1;

    frame_num = 0;
    animar_temp.iniciar();
}

// Establece los controles para el tanque
void Tanque::fijarControles(int controles[]) {
    this->controles = controles;
}

bool Tanque::inicializar() {
    // Cargar textura del tanque
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

    // Definier controles
    control_config[0][MOVER_ARRIBA]    = SDLK_UP;
    control_config[0][MOVER_ABAJO]     = SDLK_DOWN;
    control_config[0][MOVER_IZQUIERDA] = SDLK_LEFT;
    control_config[0][MOVER_DERECHA]   = SDLK_RIGHT;
    control_config[0][DISPARAR]        = SDLK_SPACE;

    control_config[1][MOVER_ARRIBA]    = SDLK_w;
    control_config[1][MOVER_ABAJO]     = SDLK_s;
    control_config[1][MOVER_IZQUIERDA] = SDLK_a;
    control_config[1][MOVER_DERECHA]   = SDLK_d;
    control_config[1][DISPARAR]        = SDLK_r;

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
                fijarAreaColision(&rect);
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
        SDL_Rect sig_rect;
        SDL_Rect colision_area;
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
        } else if (pos_x > (vista_juego.w - rect.w)) {
            pos_x = vista_juego.w - rect.w;
        }

        if (pos_y < 0) {
            pos_y = 0;
        } else if (pos_y > (vista_juego.h - rect.h)) {
            pos_y = vista_juego.h - rect.h;
        }

        // Colisiones con objetos y bloques
        sig_rect = rect;

        sig_rect.x = (int)pos_x;
        sig_rect.y = (int)pos_y;

        bloques = Escenario::obtenerBloquesEnColision(sig_rect);
         
        if (comprobarColision(&sig_rect, &colision_area) || bloques.size() > 0) {
            fijarVelocidad(0);
            
            if (bloques.size() > 0) {
                // Obtener area del primer bloque como referencia
                colision_area.x = bloques[0].x * TAMANO_BLOQUE;
                colision_area.y = bloques[0].y * TAMANO_BLOQUE;

                colision_area.w = TAMANO_BLOQUE;
                colision_area.h = TAMANO_BLOQUE;
            }

            // Truncar movimiento
            switch (direccion) {
                case ARRIBA: pos_y = colision_area.y + colision_area.h;
                    break;
                case ABAJO: pos_y = colision_area.y - rect.h;
                    break;
                case DERECHA: pos_x = colision_area.x - rect.w;
                    break;
                case IZQUIERDA: pos_x = colision_area.x + colision_area.w;
                    break;
                default: ;
            }
        }

        rect.x = (int)pos_x;
        rect.y = (int)pos_y;
    }
}

bool Tanque::manejarEvento(SDL_Event &evento, Uint8 *buffer, int *num_bytes) {
    static Paquete paquete;

    bool mover = true;
    int num_evento = -1;
    
    SDL_Keycode tecla;

    if (estado == TQ_ST_MOVER && evento.type == SDL_KEYDOWN && evento.key.repeat == 0) {
        tecla = evento.key.keysym.sym;

        if (tecla == controles[MOVER_ARRIBA]) {
            fijarDireccion(ARRIBA);
            num_evento = ARRIBA;

        } else if (tecla == controles[MOVER_ABAJO]) {
            fijarDireccion(ABAJO);
            num_evento = ABAJO;

        } else if (tecla == controles[MOVER_IZQUIERDA]) {
            fijarDireccion(IZQUIERDA);
            num_evento = IZQUIERDA;

        } else if (tecla == controles[MOVER_DERECHA]) {
            fijarDireccion(DERECHA);
            num_evento = DERECHA;

        } else if (tecla == controles[DISPARAR]) {
            disparar();
            mover=false;
            num_evento = 4;
        } else {
            mover = false;
        }

        if (mover) {
            tecla_actual = tecla;
            fijarVelocidad(100);
        }

        if (buffer && num_evento != -1) {
            // Crear paquete de evento
            *num_bytes = paquete.nuevoPqtEvento(buffer, rect.x, rect.y, (Uint8)num_evento, velocidad);
            return true;
        }
    } else if (evento.type == SDL_KEYUP && evento.key.repeat == 0) {
        tecla = evento.key.keysym.sym;

        if (tecla == tecla_actual) {
            tecla_actual = -1;
            fijarVelocidad(0);

            if (buffer) {
                // Crear paquete de evento
                *num_bytes = paquete.nuevoPqtEvento(buffer, rect.x, rect.y, (Uint8)direccion, velocidad);        
                return true;
            }
        }
    }

    return false;
}

void Tanque::disparar() {
    int x, y;
    double r, h;
    r = angulo * PI/180;

    h = (double)rect.h / 2;

    x = rect.x + (rect.w / 2 - bala[0].obtenerAncho() / 2);
    y = rect.y + (rect.h / 2 - bala[0].obtenerAlto() / 2);

    x += sin(r) * h;
    y -= cos(r) * h;

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
    fijarAreaColision(NULL);
    fijarVelocidad(0);

    frame_num = 0;
}