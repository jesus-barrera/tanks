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

// Botones
enum {
	MOVER_ARRIBA,
	MOVER_ABAJO,
	MOVER_IZQUIERDA,
	MOVER_DERECHA,
	DISPARAR,
	NUM_ACCIONES
};

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
	int estado;

	int num_vidas;
	SDL_Point init_pos;
	direccion_t init_direccion;

	// textura para la animación de movimiento
	static SDL_Texture *mover_sprite;

	// Clips para la animación de movimiento
	static SDL_Rect mover_clips[TQ_NUM_FRAMES_MOVER];

	// textura para la animación de explosión
	static SDL_Texture *explosion_sprite;

	// Clips para la explosión
	static SDL_Rect explosion_clips[TQ_NUM_FRAMES_EXPLOSION];

	// Configuración del control
	int *controles;

	// Tecla actual presionada
	SDL_Keycode tecla_actual;

	// Número de frame
	int frame_num;

	// Temporizador para la animación
	Temporizador animar_temp;

	// Temporizador para la reaparición
	Temporizador reaparecer_temp;

	// bool esAnimationFrame()
	bool comprobarAnim();

public:
	int tipo;
	// Configuraciones del control
	static int control_config[2][NUM_ACCIONES];

    Bala bala[MAX_BALAS];

    int balasdisparadas;

	// Inicializa el tanque
	Tanque(int tipo = TQ_TIPO_ROJO);

	// Establece los controles para el tanque
	void fijarControles(int controles[]);

	// Carga recursos necesarios
	static bool inicializar();

	// Libera los recursos
	static void liberarMemoria();

	// Llamado en cada ciclo de juego
	void actualizar();

	// Mueve el tanque segun su velocidad y dirección
	void mover();

	// Manejar evento
	bool manejarEvento(SDL_Event& evento, Uint8 *buffer = NULL, int *num_bytes = NULL);

	void disparar();

	// Manejar evento de colisión
	void enColision(Colisionador *objeto);

	// Renderizar
	void renderizar();

	// Destruir el tanque
	void destruir();

	void fijarNumVidas(int vidas);
	int obtenerNumVidas();
	void capturarEstado();
};


#endif // _TANK_INCLUDE_
