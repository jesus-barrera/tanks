#include "../include/Editor.h"

int Editor::bloque_seleccionado = BLOQUE_BRICK;
int Editor::tamano_pincel = PINCEL_MIN;

Tanque *Editor::jugador_1;
Tanque *Editor::jugador_2;
Base *Editor::base_1;
Base *Editor::base_2;

Objeto *Editor::objeto_seleccionado = NULL;

Boton *Editor::botones[EDITOR_NUM_BTN];

char *btn_etiquetas[EDITOR_NUM_BTN] = {
	"Limpiar",
	"Cargar",
	"Guardar",
	"Volver"
};

bool Editor::inicializar() {
	int btn_y;
	int btn_x;

	jugador_1 = new Tanque(TQ_TIPO_ROJO);
	jugador_2 = new Tanque(TQ_TIPO_AZUL);

	base_1 = new Base(BASE_TIPO_ROJO);
	base_2 = new Base(BASE_TIPO_AZUL);

	// Crear botones
	btn_y = VENTANA_ALTO - 50 * EDITOR_NUM_BTN;
	btn_x = 10;

	for (int i = 0; i < EDITOR_NUM_BTN; i++) {
		Editor::botones[i] = new Boton(btn_etiquetas[i], btn_x, btn_y);
		Editor::botones[i]->setViewport(&vista_estatus);

		btn_y += 50;
	}

	return true;
}

void Editor::liberarMemoria() {
	for (int i = 0; i < EDITOR_NUM_BTN; i++) {
		delete(botones[i]);
	}

	delete(jugador_1);
	delete(jugador_2);
	delete(base_1);
	delete(base_2);
}

/*
 * Debe llamarse cada vez que se entra a la vista de edición.
 * Limpia el mapa y reposiciona los objetos.
 */
void Editor::entrar() {
	Escenario::limpiarMapa();

	// Reposicionar objetos
	jugador_1->fijarDireccion(ARRIBA);
	jugador_1->fijarPosicion(0, 0);
	
	jugador_2->fijarDireccion(ARRIBA);
	jugador_2->fijarPosicion(0, 0);

	base_1->fijarDireccion(ARRIBA);
	base_1->fijarPosicion(0, 0);
	
	base_2->fijarDireccion(ARRIBA);
	base_2->fijarPosicion(0, 0);
}

/*
 * Llamada en cada ciclo del juego
 */
void Editor::actualizar() {
	SDL_Rect bloque_rect;
	SDL_Texture *bloque;

	SDL_RenderSetViewport(renderer_principal, &vista_juego);

	// Renderizar fondo y bloques
	Escenario::renderizar();

	// Renderizar objetos
	base_1->renderizar();
	base_2->renderizar();
	jugador_1->renderizar();
	jugador_2->renderizar();

	SDL_RenderSetViewport(renderer_principal, &vista_estatus);

	// Renderizar botones
	for (int i = 0; i < EDITOR_NUM_BTN; i++) {
		Editor::botones[i]->renderizar();
	}

	// Renderizar bloque seleccionado
	bloque = Escenario::obtenerTexturaBloque(bloque_seleccionado);
	
	bloque_rect.w = 50;
	bloque_rect.h = 50;

	bloque_rect.x = vista_estatus.w / 2 - bloque_rect.w / 2;
	bloque_rect.y = vista_estatus.h / 2 - bloque_rect.h / 2;

	SDL_RenderCopy(renderer_principal, bloque, NULL, &bloque_rect);
}

/*
 * Se encarga de manejar los eventos del mouse y teclado
 */
void Editor::manejarEvento(SDL_Event &evento) {
	if (evento.type == SDL_MOUSEBUTTONDOWN) {
		int boton = evento.button.button;

		if (objeto_seleccionado not_eq NULL) {
			if (boton == SDL_BUTTON_MIDDLE) {
				objeto_seleccionado->rotar(DERECHA);
			} else {
				insertarObjeto();
				objeto_seleccionado = NULL;
			}
		} else {		
			if (boton == SDL_BUTTON_LEFT) {
				dibujar(bloque_seleccionado);
			} else if (boton == SDL_BUTTON_RIGHT) {
				dibujar(NO_BLOQUE);
			}
		}

		// Revisar si se presionó algun botón
		switch (Boton::obtenerBotonSeleccionado(Editor::botones, EDITOR_NUM_BTN)) {
			case EDITOR_BTN_LIMPIAR: 
				Escenario::limpiarMapa(); 
				break;
			case EDITOR_BTN_CARGAR: 
				cargarMapa(); 
				break;
			case EDITOR_BTN_GUARDAR:
				guardarMapa();
				break;
			case EDITOR_BTN_SALIR:
				irAEscena("menu");
				;
				break;
		}
	} else if (evento.type == SDL_MOUSEMOTION) {
		if (objeto_seleccionado != NULL) {
			insertarObjeto();
		} else {	
			int estado_mouse = SDL_GetMouseState(NULL, NULL);
			
			if (estado_mouse & SDL_BUTTON(SDL_BUTTON_LEFT)) {
				dibujar(bloque_seleccionado);
			} else if (estado_mouse & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
				dibujar(NO_BLOQUE);
			}
		}
	} else if (evento.type == SDL_KEYDOWN && evento.key.repeat == 0) {
		objeto_seleccionado = NULL;

		switch (evento.key.keysym.sym) {
			case SDLK_1:
				bloque_seleccionado = BLOQUE_BRICK;
				break;
			case SDLK_2:
				bloque_seleccionado = BLOQUE_METAL;
				break;
			case SDLK_3:
				bloque_seleccionado = BLOQUE_AGUA_1;
				break;
			case SDLK_4:
				bloque_seleccionado = BLOQUE_ARBUSTO;
				break;
			case SDLK_w:
				if (++tamano_pincel > PINCEL_MAX) {
					tamano_pincel = PINCEL_MAX;
				}
				break;
			case SDLK_q:
				if (--tamano_pincel < PINCEL_MIN) {
					tamano_pincel = PINCEL_MIN;
				}
				break;
			case SDLK_z:
					objeto_seleccionado = jugador_1;
				break;
			case SDLK_x:
					objeto_seleccionado = jugador_2;
				break;
			case SDLK_c:
					objeto_seleccionado = base_1;
				break;
			case SDLK_v:
					objeto_seleccionado = base_2;
				break;
			default: 
				;
		}
	}
}

void Editor::insertarObjeto() {
	SDL_Point mouse_pos, bloque_pos;

	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

	objeto_seleccionado->fijarPosicion(mouse_pos.x - (mouse_pos.x % TAMANO_BLOQUE), // Alinear con la cuadrícula
									   mouse_pos.y - (mouse_pos.y % TAMANO_BLOQUE));
}

void Editor::dibujar(int bloque) {
	int i, j;
	SDL_Point mouse_pos, bloque_pos, dibujar_pos;

	SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

	// Bloque sobre el que se encuentra el puntero
	bloque_pos = Escenario::obtenerBloqueEnPunto(mouse_pos.x, mouse_pos.y);

	// Alinear bloque a cuadrícula virtual
	bloque_pos.x -= (bloque_pos.x % tamano_pincel);
	bloque_pos.y -= (bloque_pos.y % tamano_pincel);

	for (i = 0; i < tamano_pincel; i++) {
		for (j = 0; j < tamano_pincel; j++) {
			dibujar_pos.x = bloque_pos.x + i;
			dibujar_pos.y = bloque_pos.y + j;

			Escenario::insertarBloque(dibujar_pos, bloque);
		}
	}
}

void Editor::cargarMapa() {
	string nombre;
	cout << "ingresa el nombre del archivo: ";

	getline(cin, nombre);

	cargarMapa(nombre.c_str(), jugador_1, base_1, jugador_2, base_2);
}

void Editor::cargarMapa(const char *nombre_archivo, Tanque *t1, Base *b1, Tanque *t2, Base *b2) {
	int i, j;
	unsigned char byte;
	ifstream input;
	input.open(nombre_archivo, ios::in | ios::binary);

	cargarObjetoInfo(input, (t1) ? t1 : jugador_1);
	cargarObjetoInfo(input, (b1) ? b1 : base_1);
	cargarObjetoInfo(input, (t2) ? t2 : jugador_2);
	cargarObjetoInfo(input, (b2) ? b2 : base_2);

	for (i = 0; i < MAPA_FILAS; i++) {
		for (j = 0; j < MAPA_COLUMNAS; j++) {
			input.read(reinterpret_cast<char *>(&byte), sizeof(byte));

			Escenario::insertarBloque({j, i}, byte);
		}
	}

	input.close();
}

void Editor::guardarMapa() {
	int i, j;
	unsigned char byte;
	ofstream output;
	string nombre;
	cout << "ingresa el nombre del archivo: ";

	getline(cin, nombre);

	output.open(nombre.c_str(), ios::out | ios::binary);

	guardarObjetoInfo(output, jugador_1);
	guardarObjetoInfo(output, base_1);
	guardarObjetoInfo(output, jugador_2);
	guardarObjetoInfo(output, base_2);

	for (i = 0; i < MAPA_FILAS; i++) {
		for (j = 0; j < MAPA_COLUMNAS; j++) {
			byte = Escenario::obtenerBloque({j, i});
			output.write(reinterpret_cast<char *>(&byte), sizeof(byte));
		}
	}

	output.close();
}

void Editor::guardarObjetoInfo(ofstream &output, Objeto *objeto) {
	SDL_Point posicion;
	short direccion;

	posicion = objeto->obtenerPosicion();
	direccion = (short)objeto->obtenerDireccion();

	output.write(reinterpret_cast<char*>(&posicion.x), sizeof(posicion.x));
	output.write(reinterpret_cast<char*>(&posicion.y), sizeof(posicion.y));
	output.write(reinterpret_cast<char*>(&direccion), sizeof(direccion));
}

void Editor::cargarObjetoInfo(ifstream &input, Objeto *objeto) {
	SDL_Point posicion;
	short direccion;

	input.read(reinterpret_cast<char*>(&posicion.x), sizeof(posicion.x));
	input.read(reinterpret_cast<char*>(&posicion.y), sizeof(posicion.y));
	input.read(reinterpret_cast<char*>(&direccion), sizeof(direccion));

	objeto->fijarPosicion(posicion.x, posicion.y);
	objeto->fijarDireccion((direccion_t) direccion);
}

