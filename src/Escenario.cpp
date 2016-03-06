#include "../include/Escenario.h";

SDL_Texture *Escenario::textura_suelo;
SDL_Texture *Escenario::bloques[NUM_BLOQUES];
int Escenario::mapa[MAPA_FILAS][MAPA_COLUMNAS];
Temporizador Escenario::animar_temp;

bool Escenario::inicializar() {	
	textura_suelo = cargarTextura("media/textures/ground_1.png");
	bloques[BLOQUE_BRICK] = cargarTextura("media/textures/bloque_ladrillo.png");
	bloques[BLOQUE_METAL] = cargarTextura("media/textures/bloque_metal.png");
	bloques[BLOQUE_AGUA_1] = cargarTextura("media/textures/bloque_agua_1.png");
	bloques[BLOQUE_AGUA_2] = cargarTextura("media/textures/bloque_agua_2.png");
	bloques[BLOQUE_ARBUSTO] = cargarTextura("media/textures/bloque_arbusto.png");

	limpiarMapa();

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

void Escenario::renderizar() {
	renderizarFondo();
	renderizarMapa();
}

vector<SDL_Point> Escenario::obtenerBloquesEnColision(SDL_Rect &rect) {
	int x1, x2, y1, y2, x, y;
	SDL_Point bloque;
	vector<SDL_Point> bloques;

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

SDL_Point Escenario::puntoAPosicionMapa(int x, int y) {
	return {x / TAMANO_BLOQUE, y / TAMANO_BLOQUE};
}

void Escenario::destruirBloque(SDL_Point bloque_pos) {
	int bloque = mapa[bloque_pos.y][bloque_pos.x];

	if (bloque == BLOQUE_BRICK) {
		mapa[bloque_pos.y][bloque_pos.x] = NO_BLOQUE;
	}
}

void Escenario::insertarBloque(SDL_Point posicion, int bloque) {
	if (posicion.x >= 0 && posicion.x < MAPA_COLUMNAS &&
		posicion.y >= 0 && posicion.y < MAPA_FILAS) {

		mapa[posicion.y][posicion.x] = bloque;
	}
}

void Escenario::cargarMapa() {
	int i, j;
	unsigned char byte;
	ifstream input;
	string nombre;
	cout << "ingresa el nombre del archivo: ";

	getline(cin, nombre);

	input.open(nombre.c_str(), ios::in | ios::binary);

	for (i = 0; i < MAPA_FILAS; i++) {
		for (j = 0; j < MAPA_COLUMNAS; j++) {
			input.read((char *)&byte, sizeof(byte));

			mapa[i][j] = byte;
		}
	}

	input.close();
}

void Escenario::guardarMapa() {
	int i, j;
	unsigned char byte;
	ofstream output;
	string nombre;
	cout << "ingresa el nombre del archivo: ";

	getline(cin, nombre);

	output.open(nombre.c_str(), ios::out | ios::binary);

	for (i = 0; i < MAPA_FILAS; i++) {
		for (j = 0; j < MAPA_COLUMNAS; j++) {
			byte = mapa[i][j];
			output.write((char *)&byte, sizeof(byte));
		}
	}

	output.close();
}

void Escenario::limpiarMapa() {
	int i, j;

	for (i = 0; i < MAPA_FILAS; i++) {
		for (j = 0; j < MAPA_COLUMNAS; j++) {
			mapa[i][j] = NO_BLOQUE;
		}
	}
}

SDL_Texture *Escenario::obtenerTexturaBloque(int bloque) {
	if (bloque >= 0 && bloque < NUM_BLOQUES) {
		return bloques[bloque];
	}

	return NULL;
}