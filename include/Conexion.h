#ifndef CONEXION_H_INCLUDED
#define CONEXION_H_INCLUDED

#include <SDL.h>
#include "Escena.h"
#include "Boton.h"
#include "Temporizador.h"
#include "Paquete.h"

#define RUTA_MAPA_COMPARTIDO "mapas/user/compartido.map"

enum {
	BOTON_ACEPTAR,
	BOTON_REGRESAR,
	NUM_BTNS
};

class Conexion: public Escena {
private:
    enum {
        ST_UNIRSE,
        ST_ESPERAR_CONFIG,
        ST_ESPERAR_MAPA
    };

    static const int TIEMPO_CONEXION_PERDIDA = 5000;

    static TextInput *input_IP;
    static Etiqueta *input_NombreJugador;
    static Etiqueta *ejemplo;
    //static Boton *conexion_cancelar_btn;
	//static Boton *conexion_aceptar_btn;
	static Boton* botones[NUM_BTNS];
    Temporizador recibido_temp;

public:
    int estado;
    Uint8 buffer[1500];
    Paquete paquete;
	static bool inicializar();
	static void liberarMemoria();

	// Métodos de Escena
	void entrar();
    void actualizar();
    void renderizar();
    void manejarEvento(SDL_Event &evento);

    void guardarMapaCompartido(Uint8 *buffer, int num_bytes);
};

#endif // CONEXION_H_INCLUDED
