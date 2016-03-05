#ifndef _TANK_INCLUDE_
#define _TANK_INCLUDE_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "tipos.h"
#include "Escenario.h"
#include "Temporizador.h"

#define TQ_FRAMES_POR_SEC 20
#define TQ_NUM_FRAMES_MOVER 7
#define TQ_RUTA_MEDIOS "media/tank"

// texturas para la animación de movimiento
extern SDL_Texture *mover_sprites[TQ_NUM_FRAMES_MOVER];

class Tanque {
private:
	// Dimensiones y posicion asociadas al tanque
	SDL_Rect rect;

	// Textura usada en el renderizado
	SDL_Texture *textura;

	direccion_t direccion; 

	// Número de frame
	int frame_num;

	int velocidad;

	double angulo;

	Temporizador animar_temp;

	// Actualiza el sprite actual
	void actualizarSprite();

	void sigFrame();

public:
	// Inicializa el tanque
	Tanque(int x = 0, int y = 0, direccion_t direccion = ARRIBA);

	// Libera memoria
	~Tanque();

	// Carga imagenes y sonidos necesarios
	static bool inicializar();

	// Libera la memoria reservada al cargarse los medios
	static void liberarMemoria();

	// Llamado en cada frame
	void actualizar();

	// Mueve el tanque en la dirección sobre la que apunta
	void mover();

	// Cambia el frame actual
	void animar();

	// Manejar evento
	void manejarEvento(SDL_Event& evento);

	SDL_Rect obtenerRect();

	// Mueve el tanque a la posición indicada por x y y
	void fijarPosicion(int x, int y);

	// Establece la dirección del tanque (ARRIBA, ABAJO, IZQUIERDA, DERECHA)
	void fijarDireccion(direccion_t direccion);

	// Fija la velocidad del tanque
	void fijarVelocidad(int velocidad);

	// Renderiza el tanque en la ventana
	void renderizar();
};


#endif // _TANK_INCLUDE_
