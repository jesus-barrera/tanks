#include <ctime>
#include <sstream>

#include "../include/Escenario.h"
#include "../include/tipos.h"
#include "../include/utiles.h"
#include "../include/Musica.h"
#include "../include/Editor.h"

// Inicializar atributos estáticos
TextInput *Editor::input_nombre;
SelectorMapa *Editor::selector_mapa;
MapaInfo *Editor::mapa_info;

int Editor::estado;

int Editor::bloque_seleccionado = BLOQUE_BRICK;
int Editor::tamano_pincel = PINCEL_MIN;

Tanque *Editor::jugador_1;
Tanque *Editor::jugador_2;
Base *Editor::base_1;
Base *Editor::base_2;

Objeto *Editor::objeto_seleccionado;
Etiqueta *Editor::nombre_mapa;

Boton *Editor::botones[EDITOR_NUM_BTN];
Boton *Editor::cancelar_btn;
Boton *Editor::aceptar_btn;

string btn_etiquetas[EDITOR_NUM_BTN] = {
    "Nuevo",
    "Cargar",
    "Guardar",
    "Volver",
};

bool Editor::inicializar() {
    int btn_y;
    int btn_x;
    int btn_sep;

    // Crear objetos
    jugador_1 = new Tanque(TQ_TIPO_ROJO);
    jugador_2 = new Tanque(TQ_TIPO_AZUL);

    base_1 = new Base(BASE_TIPO_ROJO);
    base_2 = new Base(BASE_TIPO_AZUL);

    // Crear botones
    btn_sep = EDITOR_TAM_BTN + 10;
    btn_y = VENTANA_ALTO - (btn_sep * EDITOR_NUM_BTN + 10);
    btn_x = 15;

    for (int i = 0; i < EDITOR_NUM_BTN; i++) {
        Editor::botones[i] = new Boton(btn_etiquetas[i], btn_x, btn_y, EDITOR_TAM_BTN);
        Editor::botones[i]->setViewport(&vista_estatus);

        btn_y += btn_sep;
    }

    btn_y = VENTANA_ALTO - (btn_sep * 2 + 10);
    aceptar_btn = new Boton("Aceptar", btn_x,  btn_y, EDITOR_TAM_BTN);
    aceptar_btn->setViewport(&vista_estatus);
    cancelar_btn = new Boton("Cancelar", btn_x, btn_y +  btn_sep, EDITOR_TAM_BTN);
    cancelar_btn->setViewport(&vista_estatus);

    // Crear otros
    Editor::nombre_mapa   = new Etiqueta("", 15);

    #ifdef PUBLICAR_MAPA
    Editor::selector_mapa = new SelectorMapa();
    #else
    Editor::selector_mapa = new SelectorMapa(SelectorMapa::MAPAS_USUARIO);
    #endif

    Editor::selector_mapa->fijarPosicion(VENTANA_PADDING, VENTANA_PADDING);

    Editor::input_nombre  = new TextInput("Nombre del mapa: ", 15, VENTANA_ALTO - 35, MAXLEN_NOMBRE_MAPA);

    return true;
}

void Editor::liberarMemoria() {
    delete(jugador_1);
    delete(jugador_2);
    delete(base_1);
    delete(base_2);

    for (int i = 0; i < EDITOR_NUM_BTN; i++) {
        delete(botones[i]);
    }

    delete(cancelar_btn);
    delete(aceptar_btn);

    delete(nombre_mapa);
    delete(selector_mapa);
    delete(input_nombre);
}

/*
 * Debe llamarse cada vez que se entra a la vista de edición.
 * Limpia el mapa y reposiciona los objetos.
 */
void Editor::entrar() {
    cambiarMusicaFondo(MusicaFondoCrearMapa);
    ReproducirMusicaFondo();
    Escenario::limpiarMapa();
    nombre_mapa->fijarTexto("");
    mapa_info = NULL;
    objeto_seleccionado = NULL;

    estado = EDITOR_ST_EDITAR;

    selector_mapa->cargarMapasInfo();

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

void Editor::actualizar() { }

/*
 * Llamada en cada ciclo del juego, renderiza la escena.
 */
void Editor::renderizar() {
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

    if (estado == EDITOR_ST_SELEC_MAPA) {
        renderizarCapaGris();
        selector_mapa->renderizar();

        SDL_RenderSetViewport(renderer_principal, &vista_estatus);
        cancelar_btn->renderizar();
    } else if (estado == EDITOR_ST_LEER) {
        renderizarCapaGris();
        input_nombre->actualizar();

        SDL_RenderSetViewport(renderer_principal, &vista_estatus);
        cancelar_btn->renderizar();
        aceptar_btn->renderizar();
    } else {
        SDL_RenderSetViewport(renderer_principal, &vista_estatus);

        // Renderizar nombre del mapa
        nombre_mapa->renderizar();

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
    }

    SDL_RenderCopy(renderer_principal, bloque, NULL, &bloque_rect);
}

/*
 * Se encarga de manejar los eventos del mouse y teclado
 */
void Editor::manejarEvento(SDL_Event &evento) {
    if (estado == EDITOR_ST_EDITAR) {
        editarManejarEvento(evento);
    } else if (estado == EDITOR_ST_SELEC_MAPA) {
        selecMapaManejarEvento(evento);
    } else if (estado == EDITOR_ST_LEER) {
        inputManejarEvento(evento);
    }
}

/**
 * Manejar evento mientras se edita
 */
void Editor::editarManejarEvento(SDL_Event &evento) {
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
                Editor::entrar();
                break;
            case EDITOR_BTN_CARGAR:
                cargarMapa();
                break;
            case EDITOR_BTN_GUARDAR:
                botonGuardarPresionado();
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

/**
 * Manejar evento mientras se selecciona un mapa
 */
void Editor::selecMapaManejarEvento(SDL_Event &evento) {
    if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT) {
        mapa_info = selector_mapa->obtenerMapaSelecInfo();

        if (mapa_info) {
            Editor::cargarMapa(mapa_info->ruta, jugador_1, base_1, jugador_2, base_2);
            nombre_mapa->fijarTexto((string)(mapa_info->nombre));

            estado = EDITOR_ST_EDITAR; // ir a editar
        } else if (cancelar_btn->isMouseOver()) {
            estado = EDITOR_ST_EDITAR;
        }
    }
}

/**
 * Manejar evento mientras se escribe
 */
void Editor::inputManejarEvento(SDL_Event &evento) {
    if (evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT) {
        if (aceptar_btn->isMouseOver() && !input_nombre->vacio()) {
            crearMapa(input_nombre->obtenerTexto());

            // ir a editar
            estado = EDITOR_ST_EDITAR;
            SDL_StopTextInput();
        } else if (cancelar_btn->isMouseOver()) {
            estado = EDITOR_ST_EDITAR;
            SDL_StopTextInput();
        }
    } else {
        input_nombre->manejarEvento(evento);
    }
}

/**
 * Inserta el objeto seleccionado en la posición del mouse
 */
void Editor::insertarObjeto() {
    SDL_Point mouse_pos;

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
    estado = EDITOR_ST_SELEC_MAPA;
}

void Editor::cargarMapa(const char *nombre_archivo, Tanque *t1, Base *b1, Tanque *t2, Base *b2) {
    int i, j;
    Uint8 bloque;
    SDL_RWops *archivo;

    archivo = SDL_RWFromFile(nombre_archivo, "rb");

    if (archivo) {
        cargarObjetoInfo(archivo, (t1) ? t1 : jugador_1);
        cargarObjetoInfo(archivo, (b1) ? b1 : base_1);
        cargarObjetoInfo(archivo, (t2) ? t2 : jugador_2);
        cargarObjetoInfo(archivo, (b2) ? b2 : base_2);

        for (i = 0; i < MAPA_FILAS; i++) {
            for (j = 0; j < MAPA_COLUMNAS; j++) {
                SDL_RWread(archivo, &bloque, sizeof(bloque), 1);
                Escenario::insertarBloque({j, i}, bloque);
            }
        }

        SDL_RWclose(archivo);
    }
}

void Editor::botonGuardarPresionado() {
    if (mapa_info) {
        // Mapa cargado, sobreescribir
        guardarMapa(mapa_info->ruta);
    } else {
        // Nuevo mapa, solicitar nombre del mapa
        SDL_StartTextInput();
        estado = EDITOR_ST_LEER;
    }
}

void Editor::crearMapa(string nombre) {
    stringstream ssruta;
    string ruta;
    SDL_bool en_juego;

    // Generar ruta del archivo
    #ifdef PUBLICAR_MAPA
    ssruta << MAPAS_RUTA << time(NULL) << MAPA_TIPO;
    en_juego = SDL_TRUE;
    #else
    ssruta << MAPAS_USUARIO_RUTA << time(NULL) << MAPA_TIPO;
    en_juego = SDL_FALSE;
    #endif

    ruta = ssruta.str();

    guardarMapa(ruta.c_str());
    mapa_info = selector_mapa->agregar(nombre, ruta, en_juego);
    nombre_mapa->fijarTexto((string)mapa_info->nombre);
}

void Editor::guardarMapa(const char *nombre_archivo) {
    int i, j;
    Uint8 bloque;
    SDL_RWops *archivo;

    archivo = SDL_RWFromFile(nombre_archivo, "wb");

    guardarObjetoInfo(archivo, jugador_1);
    guardarObjetoInfo(archivo, base_1);
    guardarObjetoInfo(archivo, jugador_2);
    guardarObjetoInfo(archivo, base_2);

    for (i = 0; i < MAPA_FILAS; i++) {
        for (j = 0; j < MAPA_COLUMNAS; j++) {
            bloque = Escenario::obtenerBloque({j, i});
            SDL_RWwrite(archivo, &bloque, sizeof(bloque), 1);
        }
    }

    SDL_RWclose(archivo);
}

void Editor::guardarObjetoInfo(SDL_RWops *archivo, Objeto *objeto) {
    SDL_Point posicion;
    Uint16 direccion;

    posicion = objeto->obtenerPosicion();
    direccion = (Uint16)objeto->obtenerDireccion();

    SDL_RWwrite(archivo, &posicion.x, sizeof(posicion.x), 1);
    SDL_RWwrite(archivo, &posicion.y, sizeof(posicion.y), 1);
    SDL_RWwrite(archivo, &direccion, sizeof(direccion), 1);
}

void Editor::cargarObjetoInfo(SDL_RWops *archivo, Objeto *objeto) {
    SDL_Point posicion;
    Uint16 direccion;

    SDL_RWread(archivo, &posicion.x, sizeof(posicion.x), 1);
    SDL_RWread(archivo, &posicion.y, sizeof(posicion.y), 1);
    SDL_RWread(archivo, &direccion, sizeof(direccion), 1);

    objeto->fijarPosicion(posicion.x, posicion.y);
    objeto->fijarDireccion((direccion_t) direccion);
}

