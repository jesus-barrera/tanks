#include "../include/utiles.h"
#include "../include/Editor.h"
#include "../include/Escenario.h"
#include "../include/Menu.h"
#include "../include/Musica.h"

string etiquetas_botones[NUM_BOTONES] = {
    "Nueva partida",
    "Buscar partida",
    "Crear mapa",
    "Salir"
};

Boton *Menu::botones[NUM_BOTONES];

void Menu::entrar() {
    cambiarMusicaFondo(MusicaFondoCrearMapa);
    ReproducirMusicaFondo();
    Editor::cargarMapa(MAPAS_RUTA"1462739187.map");
}

void Menu::actualizar() { }

void Menu::renderizar() {
    SDL_RenderSetViewport(renderer_principal, &vista_juego);

    // Renderizar botones
    for (int i = 0; i < NUM_BOTONES; i++) {
        botones[i]->renderizar();
    }
}

void Menu::manejarEvento(SDL_Event &evento) {
    if (evento.type == SDL_MOUSEBUTTONDOWN) {
        switch (Boton::obtenerBotonSeleccionado(Menu::botones, NUM_BOTONES)) {
            case BOTON_INICIAR:
                ReproducirSonido(Snd_Click_boton, 100, 0, 0);
                irAEscena("nueva-partida");
                break;
            case BOTON_CONECTAR:
                ReproducirSonido(Snd_Click_boton, 100, 0, 0);
                irAEscena("conectar");
                break;
            case BOTON_EDITAR:
                ReproducirSonido(Snd_Click_boton, 100, 0, 0);
                irAEscena("editar");
                break;
            case BOTON_SALIR:
                ReproducirSonido(Snd_Click_boton, 100, 0, 0);
                terminarJuego();
                break;
            default:
                ;
        }
    }
}

bool Menu::inicializar() {
    int btn_x, btn_y;

    btn_y = VENTANA_ALTO - 50 * NUM_BOTONES;
    btn_x = 30;

    for (int i = 0; i < NUM_BOTONES; i++) {
        botones[i] = new Boton(etiquetas_botones[i], btn_x, btn_y, MENU_BTN_TAM);

        btn_y += 50;
    }

    return true;
}

void Menu::liberarMemoria() {
    for (int i = 0; i < NUM_BOTONES; i++) {
        delete(botones[i]);
    }
}
