#include "../include/Escenario.h";

SDL_Texture *Escenario::textura_suelo;
SDL_Texture *Escenario::bloques[NUM_BLOQUES];
int Escenario::mapa[MAPA_FILAS][MAPA_COLUMNAS];
Temporizador Escenario::animar_temp;

bool Escenario::inicializar() {
	srand(time(NULL));
	
	textura_suelo = cargarTextura("media/textures/ground_1.png");
	bloques[BLOQUE_BRICK_1] = cargarTextura("media/textures/bloque_brick_1.png");
	bloques[BLOQUE_BRICK_2] = cargarTextura("media/textures/bloque_brick_2.png");
	bloques[BLOQUE_BRICK_3] = cargarTextura("media/textures/bloque_brick_3.png");
	bloques[BLOQUE_METAL] = cargarTextura("media/textures/bloque_metal.png");
	bloques[BLOQUE_AGUA_1] = cargarTextura("media/textures/bloque_agua_1.png");
	bloques[BLOQUE_AGUA_2] = cargarTextura("media/textures/bloque_agua_2.png");
	bloques[BLOQUE_ARBOL] = cargarTextura("media/textures/bloque_arbol.png");

	animar_temp.iniciar();

	return true;
}

void Escenario::liberarMemoria() {
	for (int i = 0; i < NUM_BLOQUES; i++) {
		SDL_DestroyTexture(bloques[i]);
		bloques[i] = NULL;
	}

	SDL_DestroyTexture(textura_suelo);
	textura_suelo = NULL;
}

void Escenario::crearMapaAleatorio() {
	int x, y;

	for (y = 0; y < MAPA_FILAS; y++) {
		for (x = 0; x < MAPA_COLUMNAS; x++) {
			mapa[y][x] = NO_BLOQUE;

			if (rand() % 50 > 40) {
				mapa[y][x] = rand() % 7 + 1;
			}
		}
	}
}

bool Escenario::cargarMapaDesdeArchivo(char *nombre_archivo) {
	int i, j;
	unsigned char byte;
	ifstream input;
	
	input.open(nombre_archivo, ios::in | ios::binary);

	if (input.is_open()) {
		for (i = 0; i < MAPA_FILAS; i++) {
			for (j = 0; j < MAPA_COLUMNAS; j++) {
				input.read((char *)&byte, sizeof(byte));

				mapa[i][j] = byte;
			}
		}

		input.close();

		return true;
	}

	return false;
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
	bool animar_agua;
	int x, y;
	SDL_Rect rect;

	rect.w = TAMANO_BLOQUE;
	rect.h = TAMANO_BLOQUE;

	animar_agua = animar_temp.obtenerTiempo() >= 1000;

	for (y = 0; y < MAPA_FILAS; y++) {
		rect.y = y * rect.h;

		for (x = 0; x < MAPA_COLUMNAS; x++) {
			rect.x = x * rect.w;
			
			if (mapa[y][x]) {

				if ((mapa[y][x] == BLOQUE_AGUA_1 || mapa[y][x] == BLOQUE_AGUA_2) && animar_agua) {
					if (mapa[y][x] == BLOQUE_AGUA_1) {
						mapa[y][x] = BLOQUE_AGUA_2;
					} else {
						mapa[y][x] = BLOQUE_AGUA_1;
					}
				}

				SDL_RenderCopy(renderer_principal, bloques[mapa[y][x]], NULL, &rect);
			}
		}
	}

	if (animar_agua) {
		animar_temp.iniciar();
	}
}

vector<bloque_pos> Escenario::obtenerBloquesEnColision(SDL_Rect &rect) {
	int x1, x2, y1, y2, x, y;
	bloque_pos bloque;
	vector<bloque_pos> bloques;

	x1 = rect.x / TAMANO_BLOQUE;
	x2 = ((rect.x + rect.w) / TAMANO_BLOQUE) % MAPA_COLUMNAS;

	y1 = rect.y / TAMANO_BLOQUE;
	y2 = ((rect.y + rect.h) / TAMANO_BLOQUE) % MAPA_FILAS;

	for (y = y1; y <= y2; y++) {
		for (x = x1; x <= x2; x++) {
			if (mapa[y][x] != NO_BLOQUE) {
				bloque.y = y;
				bloque.x = x;

				bloques.push_back(bloque);
			}
		}
	}

	return bloques;
}

void Escenario::destruirBloque(bloque_pos position) {
	int bloque = mapa[position.y][position.x];

	if (bloque >= BLOQUE_BRICK_1 && bloque <= BLOQUE_BRICK_3) {
		if (bloque == BLOQUE_BRICK_3) {
			mapa[position.y][position.x] = NO_BLOQUE;
		} else {
	 		mapa[position.y][position.x]++;
		}
	} else if (bloque == BLOQUE_ARBOL) {
		mapa[position.y][position.x] = NO_BLOQUE;
	}
}