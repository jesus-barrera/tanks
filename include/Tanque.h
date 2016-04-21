#ifndef _TANK_INCLUDE_
#define _TANK_INCLUDE_

#include <SDL.h>

#include "Escenario.h"
#include "Temporizador.h"
#include "Colisionador.h"
#include "Objeto.h"
#include "Bala.h"

#define TQ_TAMANO 2

#define TQ_REAPARECER_TIEMPO 1500

#define TQ_FRAMES_POR_SEC 10
#define TQ_NUM_FRAMES_MOVER 7
#define TQ_NUM_FRAMES_EXPLOSION 5

#define TQ_RUTA_MEDIOS "media/tank"
#define TQ_ETIQUETA "tank"

#define MAX_BALAS 3

enum {
	TQ_TIPO_ROJO,
	TQ_TIPO_AZUL
};

enum {
	TQ_ST_MOVER,
	TQ_ST_EXPLOTAR,
	TQ_ST_ESPERAR
};

class Tanque: public Colisionador, public Objeto {
private:
	int tipo;
	int estado;

	// textura para la animación de movimiento
	static SDL_Texture *mover_sprite;

	// Clips para la animación de movimiento
	static SDL_Rect mover_clips[TQ_NUM_FRAMES_MOVER];
	
	// textura para la animación de explosión
	static SDL_Texture *explosion_sprite;

	// Clips para la explosión
	static SDL_Rect explosion_clips[TQ_NUM_FRAMES_EXPLOSION];

	// Número de frame
	int frame_num;

	// Temporizador para la animación
	Temporizador animar_temp;

	// Temporizador para la reaparición
	Temporizador reaparecer_temp;

	// bool esAnimationFrame()
	bool comprobarAnim();
public:
    Bala bala[MAX_BALAS];
    int balasdisparadas;

	// Inicializa el tanque
	Tanque(int tipo = TQ_TIPO_ROJO);

	// Carga recursos necesarios
	static bool inicializar();
	
	// Libera los recursos
	static void liberarMemoria();
	
	// Llamado en cada ciclo de juego
	void actualizar();
	
	// Mueve el tanque segun su velocidad y dirección
	void mover();
	
	// Manejar evento
	void manejarEvento(SDL_Event& evento);
	
	// Manejar evento de colisión
	void enColision(Colisionador *objeto);

	// Renderizar
	void renderizar();

	// Destruir el tanque
	void destruir();
};


#endif // _TANK_INCLUDE_
