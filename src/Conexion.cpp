#include "../include/globales.h"
#include "../include/utiles.h"
#include "../include/Editor.h"
#include "../include/Escenario.h"
#include "../include/Conexion.h"
#include "../include/network.h"
#include "../include/Paquete.h"
#include "../include/Jugar.h"
#include "../include/Musica.h"

TextInput *Conexion::input_IP;
Etiqueta *Conexion::input_NombreJugador;
Etiqueta *Conexion::ejemplo;

string Conexion_btns[NUM_BTNS] = {
	"Aceptar",
	"Regresar"
};

Boton *Conexion::botones[NUM_BTNS];

void Conexion::entrar() {
	ejemplo->fijarTexto("Ejemplo: 255.255.255.255");
    input_IP->limpiarTexto();
    cambiarMusicaFondo(MusicaFondoCrearPartida);
    ReproducirMusicaFondo();
    //cambiarMusicaFondo(MusicaFondoCrearPartida);
    estado = ST_UNIRSE;
}

void Conexion::actualizar() {
    if (estado == ST_ESPERAR_CONFIG) {
        if (Net_recibir(buffer, 1000)) {
            paquete.analizar(buffer);

            if (paquete.tipo == PQT_CONFIGURACION) {
                Jugar *jugar = (Jugar *)obtenerEscena("jugar");

                cout << "[Debug] Configuracion: ";
                cout << "nombre=" << paquete.nombre << ", ";
                cout << "tipo= " << (int)paquete.tipo_juego << ", ";
                cout << "mapa= " << (int)paquete.info_mapa << endl;

                if (paquete.tipo_juego > 0) {
                    jugar->fijarModoJuego(Jugar::MODO_JUEGO_VIDAS);
                    jugar->fijarNumVidas(paquete.tipo_juego);
                } else {
                    jugar->fijarModoJuego(Jugar::MODO_JUEGO_BASE);
                }

                jugar->fijarModoNet(Jugar::MODO_CLIENTE);

                jugar->fijarNombreJugador(obtenerNombreEquipo());
                jugar->fijarNombreOponente((string)paquete.nombre);

                if (paquete.info_mapa >= 0) {
                    if (!jugar->cargarMapaPorId(paquete.info_mapa)) {
                        cout << "[Debug] Error al cargar el mapa" << endl;
                        return;
                    }

                    irAEscena("jugar");

                    // Enviar confirmación
                    int tam_pqt = paquete.nuevoPqtConfirmacion(buffer, "OK");
                    Net_enviar(buffer, tam_pqt);
                } else {
                    cout << "[Debug] Recibir mapa" << endl;
                }
            }
        }
    }
}

void Conexion::renderizar() {
	SDL_RenderSetViewport(renderer_principal, &vista_juego);
    Escenario::renderizar();
	renderizarCapaGris();

    input_IP->actualizar();

    input_NombreJugador->renderizar();
    ejemplo->renderizar();

	// Renderizar botones
    for (int i = 0; i < NUM_BTNS; i++) {
		botones[i]->renderizar();
	}
}

void Conexion::manejarEvento(SDL_Event &evento) {
    input_IP->manejarEvento(evento);
	if (evento.type == SDL_MOUSEBUTTONDOWN) {
		switch (Boton::obtenerBotonSeleccionado(Conexion::botones,NUM_BTNS)) {
			case BOTON_ACEPTAR:
                ReproducirSonido(Snd_Click_boton, 100, 0, 0);
                if(estado == ST_UNIRSE && !input_IP->vacio()){
                    char nombreEquipo[MAXTAM_EQUIPO];
                    int tam_pqt;
                    strncpy(nombreEquipo, obtenerNombreEquipo().c_str(), 20);

                    tam_pqt = paquete.nuevoPqtUnirse(buffer, nombreEquipo);

                    if (Net_iniciar(3454)) {
                        if (Net_resolverHost(input_IP->obtenerTexto())) {

                            Net_enviar(buffer, tam_pqt);

                            ejemplo->fijarTexto("Esperando configuracion del juego... :)");

                            estado = ST_ESPERAR_CONFIG;
                        } else {
                            ejemplo->fijarTexto("No se encontro jugador");
                        }
                    } else {
                        ejemplo->fijarTexto("Error al crear conexion");
                    }
                }
				break;
			case BOTON_REGRESAR:
                ReproducirSonido(Snd_Click_boton, 100, 0, 0);
                //SDL_StopTextInput();
				irAEscena("menu");
				Net_terminar();
				break;
			default:
				;
		}
	}
}

bool Conexion::inicializar() {
	int btn_x, btn_y;

	btn_y = VENTANA_ALTO - 50 * NUM_BTNS;
	btn_x = 15;

    for (int i = 0; i < NUM_BTNS; i++) {
		Conexion::botones[i] = new Boton(Conexion_btns[i], btn_x, VENTANA_ALTO - (50*(i+1)));
		Conexion::botones[i]->setViewport(&vista_juego);
		btn_y += 50;
	}
		//Editor::selector_mapa = new SelectorMapa();
    Conexion::input_IP = new TextInput("Direccion IP o Host: ");
    string etqt= "Nombre de jugador: "+ obtenerNombreEquipo();
    Conexion::input_NombreJugador = new Etiqueta(etqt, 0, 100);
    Conexion::ejemplo = new Etiqueta("Ejemplo: 255.255.255.255", 0, 50);

	return true;
}

void Conexion::liberarMemoria() {
	for (int i = 0; i < NUM_BTNS; i++) {
		delete(botones[i]);
	}
	Net_terminar();
}