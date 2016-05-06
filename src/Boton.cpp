#include <SDL.h>

#include "../include/globales.h"
#include "../include/utiles.h"
#include "../include/Boton.h"
#include "../include/colores.h"

SDL_Color Boton::color_principal    = COLOR_BLANCO;
SDL_Color Boton::color_seleccionado = COLOR_VERDE;
SDL_Color Boton::color_mouse_over   = COLOR_ROJO;

Boton::Boton(string texto, int x, int y, int tam_fuente) 
        : Etiqueta(texto, x, y, tam_fuente, Boton::color_principal), 
          Hoverable(&this->rect) {

    this->seleccionado = false;
    this->mouse_is_over = false;
}

void Boton::renderizar() {
    bool is_mouse_over = this->isMouseOver();

    if (!this->seleccionado && (this->mouse_is_over != is_mouse_over)) {
        // Actualizar color
        if (is_mouse_over) {
            this->fijarColor(Boton::color_mouse_over);
        } else {
            this->fijarColor(Boton::color_principal);
        }

        this->mouse_is_over = is_mouse_over;
    }

    Etiqueta::renderizar();
}

bool Boton::estaSeleccionado() {
    return this->seleccionado;
}

bool Boton::estaSeleccionado(bool seleccionado) {
    if (seleccionado != this->seleccionado) {
        if (seleccionado) {
            this->fijarColor(Boton::color_seleccionado);
        } else {
            if (this->mouse_is_over) {
                this->fijarColor(Boton::color_mouse_over);
            } else {
                this->fijarColor(Boton::color_principal);
            }
        }

        this->seleccionado = seleccionado;
    }

    return seleccionado;
}

/**
 * Encuentra el boton seleccionado en un arreglo de botones
 */
int Boton::obtenerBotonSeleccionado(Boton *botones[], int num_botones) {
    int boton_seleccionado = -1;

    for (int i = 0; i < num_botones; i++) {
        if (botones[i]->isMouseOver()) {
            boton_seleccionado = i;
            break;
        }
    }

    return boton_seleccionado;
}

/**
 * Renderiza una lista de botones
 */
void Boton::renderizarBotones(Boton *botones[], int num_botones) {
    SDL_Rect *viewport, *btn_viewport;

    viewport = &vista_juego;
    
    for (int i = 0; i < num_botones; i++) {
        btn_viewport = botones[i]->getViewport();
        
        if (btn_viewport != viewport) {
            if (btn_viewport) {
                SDL_RenderSetViewport(renderer_principal, btn_viewport);
            } else {
                SDL_RenderSetViewport(renderer_principal, &vista_juego);
            }

            viewport = btn_viewport;
        }

        botones[i]->renderizar();
    }
}

/**
 * Elimina una lista de botones instanciados con new
 */
void Boton::eliminarBotones(Boton *botones[], int num_botones) {
    for (int i = 0; i < num_botones; i++) {
        delete(botones[i]);
        botones[i] = NULL;   
    }
}