#include <cmath>

#include "../include/globales.h"
#include "../include/utiles.h"
#include "../include/Paquete.h"
#include "../include/Musica.h"
#include "../include/network.h"
#include "../include/Paquete.h"

#include "../include/Tanque.h"

static Uint8 buffer[1000];

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

void Tanque::fijarVelocidad(float velocidad){
    if(velocidad == 0){
        DetenerCanal(tipo+1);
    } else {
        ReproducirSonido(Snd_Movimiento_tanque, 100, tipo+1, -1);
    }

    this->velocidad = velocidad;
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
                // Restaurar valores
                fijarPosicion(init_pos.x, init_pos.y);
                fijarDireccion(init_direccion);
                fijarAreaColision(&rect);
                frame_num = 0;

                estado = TQ_ST_MOVER;
            }
            break;
        default: ;
    }

    for (int i = 0; i < MAX_BALAS; i++) bala[i].actualizar();
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

bool Tanque::manejarEvento(SDL_Event &evento) {
    bool mover;
    int num_evento;
    SDL_Keycode tecla;

    num_evento = -1;

    if (estado == TQ_ST_MOVER && evento.type == SDL_KEYDOWN && evento.key.repeat == 0) {
        tecla = evento.key.keysym.sym;
        mover = true;

        if (tecla == controles[MOVER_ARRIBA]) {
            fijarDireccion(ARRIBA);

        } else if (tecla == controles[MOVER_ABAJO]) {
            fijarDireccion(ABAJO);

        } else if (tecla == controles[MOVER_IZQUIERDA]) {
            fijarDireccion(IZQUIERDA);

        } else if (tecla == controles[MOVER_DERECHA]) {
            fijarDireccion(DERECHA);

        } else if (tecla == controles[DISPARAR]) {
            mover = false;

            if (disparar()) {
                num_evento = EVENTO_DISPARO;
            }

        } else {
            mover = false;
        }

        if (mover) {
            num_evento = (int)direccion;
            tecla_actual = tecla;
            fijarVelocidad(100);
        }

    } else if (evento.type == SDL_KEYUP && evento.key.repeat == 0) {
        tecla = evento.key.keysym.sym;

        if (tecla == tecla_actual) {
            tecla_actual = -1;
            fijarVelocidad(0);

            num_evento = (int)direccion;
        }
    }

    if (buffer && num_evento != -1) {
        int num_bytes = Paquete::nuevoPqtEvento(buffer, rect.x, rect.y, (Uint8)num_evento, velocidad);
        Net_enviar(buffer, num_bytes);

        return true;
    } else {
        return false;
    }
}

bool Tanque::disparar() {
    bool disparo;
    int x, y;
    double r, h;
    r = angulo * PI/180;

    h = (double)rect.h / 2;

    x = rect.x + (rect.w / 2 - bala[0].obtenerAncho() / 2);
    y = rect.y + (rect.h / 2 - bala[0].obtenerAlto() / 2);

    x += sin(r) * h;
    y -= cos(r) * h;

    disparo = false;

    for (int i = 0; i < MAX_BALAS && !disparo; i++) {
        if (bala[i].disponible) {
            bala[i].Disparar(direccion, x, y);
            disparo = true;
        }
    }

    return disparo;
}

void Tanque::enColision(Colisionador *objeto) {

}

void Tanque::renderizar() {
    switch (estado) {
        case TQ_ST_MOVER:
            textura = Tanque::mover_sprite;

            if (this->tipo == JUGADOR_1) {
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

    for (int i = 0; i < MAX_BALAS; i++) {
        bala[i].renderizar();
    }
}

void Tanque::destruir() {
    ReproducirSonido(Snd_Explosion, 100, 0, 0);

    fijarAreaColision(NULL);
    fijarVelocidad(0);
    --num_vidas;
    frame_num = 0;

    estado = TQ_ST_EXPLOTAR;
}

void Tanque::fijarNumVidas(int vidas) {
    this->num_vidas = vidas;
}

int Tanque::obtenerNumVidas() {
    return this->num_vidas;
}

void Tanque::capturarEstado() {
    // Capturar posicion para reaparecer después de destruirse
    init_pos.x = rect.x;
    init_pos.y = rect.y;
    init_direccion = direccion;

    // Reiniciar valores
    for (int i = 0; i < MAX_BALAS; i++) {
        bala[i].disponible = true;
        bala[i].fijarVelocidad(0);
    }

    fijarVelocidad(0);
    fijarAreaColision(&rect);
    frame_num = 0;
    estado = TQ_ST_MOVER;
}

void Tanque::habilitarBalasDestruccion(bool destruir) {
    for (int i = 0; i < MAX_BALAS; i++) {
        bala[i].habilitarDestruccion(destruir);
    }
}

