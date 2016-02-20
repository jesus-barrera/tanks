#include "../include/Escenario.h";

SDL_Texture *Escenario::textura_suelo;
SDL_Texture *Escenario::textura_bloque;
int Escenario::mapa[MAPA_FILAS][MAPA_COLUMNAS];

bool Escenario::inicializar() {
	srand(time(NULL));
	
	textura_suelo = cargarTextura("media/textures/ground_1.png");
	textura_bloque = cargarTextura("media/textures/bloque_1.png");

	if (textura_suelo != NULL && textura_bloque != NULL) {
		
		return true;
	} else {
		return false;
	}
}

void Escenario::liberarMemoria() {
	SDL_DestroyTexture(textura_suelo);
	textura_suelo = NULL;

	SDL_DestroyTexture(textura_bloque);
	textura_bloque = NULL;
}

void Escenario::crearMapaAleatorio() {
	int x, y;

	for (y = 0; y < MAPA_FILAS; y++) {
		for (x = 0; x < MAPA_COLUMNAS; x++) {
			mapa[y][x] = 0;

			if (rand() % 50 > 40) {
				mapa[y][x] = 5;
			}
		}
	}
}

void Escenario::renderizarFondo() {
	int i, j, x_repeat, y_repeat;
	SDL_Rect rect;

	SDL_QueryTexture(textura_suelo, NULL, NULL, &rect.w, &rect.h);

	x_repeat = vista_juego.w / rect.w;
	y_repeat = vista_juego.h / rect.h;

	for (i = 0; i <= x_repeat; i++) {
		rect.x = i * rect.w;

		for (j = 0; j <= y_repeat; j++) {
			rect.y = j * rect.h;
			
			SDL_RenderCopy(renderer_principal, textura_suelo, NULL, &rect);
		}
	}
}

void Escenario::renderizarMapa() {
	int x, y;
	SDL_Rect rect;

	rect.w = TAMANO_BLOQUE;
	rect.h = TAMANO_BLOQUE;

	for (y = 0; y < MAPA_FILAS; y++) {
		rect.y = y * rect.h;

		for (x = 0; x < MAPA_COLUMNAS; x++) {
			rect.x = x * rect.w;
			
			if (mapa[y][x]) {
				SDL_RenderCopy(renderer_principal, textura_bloque, NULL, &rect);
			}
		}
	}
}

bool Escenario::enColisionConMapa(SDL_Rect &rect) {
	int x1, x2, y1, y2, x, y;

	x1 = rect.x / TAMANO_BLOQUE;
	x2 = (rect.x + rect.w) / TAMANO_BLOQUE;

	y1 = rect.y / TAMANO_BLOQUE;
	y2 = (rect.y + rect.h) / TAMANO_BLOQUE;

	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			if (mapa[y][x]) {
				return true; // colision con el mapa
			}
		}
	}

	// no hubo colision
	return false;
}