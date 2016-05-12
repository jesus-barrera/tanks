#ifndef CONEXION_H_INCLUDED
#define CONEXION_H_INCLUDED

#include <SDL.h>
#include "Escena.h"
#include "Boton.h"
#include "Temporizador.h"
#include "Paquete.h"

enum {
	BOTON_ACEPTAR,
	BOTON_REGRESAR,
	NUM_BTNS
};

class Conexion: public Escena {
private:
    enum {
        ST_UNIRSE,
        ST_ESPERAR_CONFIG
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
    Uint8 buffer[1000];
    Paquete paquete;
	static bool inicializar();
	static void liberarMemoria();

	// Métodos de Escena
	void entrar();
    void actualizar();
    void renderizar();
    void manejarEvento(SDL_Event &evento);
};

#endif // CONEXION_H_INCLUDED
