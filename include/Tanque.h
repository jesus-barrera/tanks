#ifndef _TANK_INCLUDE_
#define _TANK_INCLUDE_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "tipos.h"
#include "Escenario.h"
#include "Temporizador.h"
#include "Objeto.h"

#define TQ_TAMANO 2
#define TQ_FRAMES_POR_SEC 20
#define TQ_NUM_FRAMES_MOVER 7
#define TQ_RUTA_MEDIOS "media/tank"

class Tanque: public Objeto {
private:
	// texturas para la animación de movimiento
	static SDL_Texture *mover_sprites[TQ_NUM_FRAMES_MOVER];

	// Número de frame
	int frame_num;

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
};


#endif // _TANK_INCLUDE_
