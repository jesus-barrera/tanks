#ifndef BALA_H_INCLUDED
#define BALA_H_INCLUDED

#include <SDL.h>

#include "globales.h"
#include "Temporizador.h"
#include "Colisionador.h"
#include "Objeto.h"

#define BALA_TAMANO 2
#define BALA_FRAMES_POR_SEC 20
#define BALA_NUM_FRAMES_MOVER 1
#define BALA_RUTA_MEDIOS "media/tank"
#define BALA_ETIQUETA "bala"

class Tanque;

class Bala: public Colisionador, public Objeto {
public:
	// texturas para la animación de movimiento
	static SDL_Texture *mover_sprites[BALA_NUM_FRAMES_MOVER];
	// Número de frame
	int frame_num;
	bool disponible=true;
	Temporizador animar_temp;
	// Actualiza el sprite actual
	void actualizarSprite();
	void sigFrame();
public:
	// Inicializa el tanque
	Bala();
	// Carga imagenes y sonidos necesarios
	static bool inicializar();
	// Libera la memoria reservada al cargarse los medios
	static void liberarMemoria();
	// Llamado en cada frame
	void actualizar();
	// Mueve el tanque en la dirección sobre la que apunta
	bool mover();
	// Cambia el frame actual
	void animar();
	// Manejar evento
	void manejarEvento(SDL_Event& evento);
	// Manejar evento de colisión con objeto
	void enColision(Colisionador *objeto);
	void Disparar(direccion_t direccion, int x, int y);
	void renderizar();
};

#endif // BALA_H_INCLUDED
