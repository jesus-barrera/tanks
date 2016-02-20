#ifndef _ESCENARIO_INCLUDE_
#define _ESCENARIO_INCLUDE_

#include <SDL.h>
#include <SDL_image.h>
#include "tipos.h"
#include "utiles.h"

static SDL_Texture *textura_suelo;
static SDL_Texture *textura_bloque;

static bool mapa[24][32];

class Escenario {
public:
	static bool inicializar() {
		textura_suelo = cargarTextura("media/textures/ground_1.png");
		textura_bloque = cargarTextura("media/textures/bloque_1.png");

		if (textura_suelo != NULL && textura_bloque != NULL) {
			inicializarMapa();

			return true;
		} else {
			return false;
		}
	}

	static void liberarMemoria() {
		SDL_DestroyTexture(textura_suelo);

		textura_suelo = NULL;
	}

	static void inicializarMapa() {
		int i, j, x_repeat, y_repeat;

		x_repeat = vista_juego.w / TAMANO_BLOQUE;
		y_repeat = vista_juego.h / TAMANO_BLOQUE;

		for (i = 0; i < x_repeat; i++) {

			for (j = 0; j < y_repeat; j++) {
				if (rand() % 50 > 40) {
					mapa[i][j] = true;
				}
			}
		}
	}

	static void renderizarFondo() {
		int i, j, x_repeat, y_repeat;
		SDL_Rect rect;

		SDL_QueryTexture(textura_suelo, NULL, NULL, &rect.w, &rect.h);

		x_repeat = VENTANA_ANCHO / rect.w;
		y_repeat = VENTANA_ALTO / rect.h;

		for (i = 0; i < x_repeat; i++) {
			rect.x = i * rect.w;

			for (j = 0; j < y_repeat; j++) {
				rect.y = j * rect.h;
				
				SDL_RenderCopy(renderer_principal, textura_suelo, NULL, &rect);
			}
		}
	}

	static void renderizarMapa() {
		int i, j, x_repeat, y_repeat;
		SDL_Rect rect;

		rect.w = TAMANO_BLOQUE;
		rect.h = TAMANO_BLOQUE;

		x_repeat = vista_juego.w / rect.w;
		y_repeat = vista_juego.h / rect.h;

		for (i = 0; i < x_repeat; i++) {
			rect.x = i * rect.w;

			for (j = 0; j < y_repeat; j++) {
				rect.y = j * rect.h;
				
				if (mapa[i][j]) {
					SDL_RenderCopy(renderer_principal, textura_bloque, NULL, &rect);
				}
			}
		}
	}
};

#endif