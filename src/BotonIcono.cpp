#include "../include/globales.h"
#include "../include/utiles.h"
#include "../include/Musica.h"
#include "../include/BotonIcono.h"

/**
 * Crea una nueva instancia de BotonIcono.
 */
BotonIcono::BotonIcono(string ruta_textura, int pos_x, int pos_y, float escala)
        : Hoverable(&this->rect) {

    this->icono = cargarTextura(ruta_textura);

    /**
     * Obtener dimensiones y escalar
     */
    SDL_QueryTexture(this->icono, NULL, NULL, &this->rect.w, &this->rect.h);

    this->rect.w *= escala;
    this->rect.h *= escala;

    this->rect.x = pos_x;
    this->rect.y = pos_y;

    this->estado_actual = NO_DEFINIDO;
    this->estado_previo = NO_DEFINIDO;
}

int BotonIcono::manejarEvento(SDL_Event &evento) {
    if (this->isMouseOver()) {
        if (evento.type == SDL_MOUSEBUTTONDOWN &&
            evento.button.button == SDL_BUTTON_LEFT) {

            ReproducirSonido(Snd_Click_boton, 100, 0, 0);

            this->estado_actual = PRESIONADO;
        } else {
            this->estado_actual = MOUSE_ENCIMA;
        }
    } else {
        this->estado_actual = MOUSE_FUERA;
    }

    return this->estado_actual;
}

void BotonIcono::renderizar() {
    bool renderizar;

    renderizar = this->estado_previo != this->estado_actual;

    // Dibujar fondo cuando el cursor esta encima
    if (renderizar &&
        (this->estado_actual == BotonIcono::MOUSE_ENCIMA ||
         this->estado_actual == BotonIcono::PRESIONADO)) {

        SDL_SetRenderDrawColor(renderer_principal, 0xFF, 0x50, 0x50, 0x35);
        SDL_RenderFillRect(renderer_principal, &this->rect);
    }

    if (renderizar) {
        SDL_RenderCopy(renderer_principal, this->icono, NULL, &this->rect);
    }
}
