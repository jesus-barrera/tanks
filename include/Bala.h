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

#define MAX_BLOQUES_EN_COLISION 2

class Bala: public Colisionador, public Objeto {
public:
	static SDL_Texture *mover_sprites[BALA_NUM_FRAMES_MOVER];
	int frame_num;
	bool disponible=true;
	bool destruccion_activa;
	Temporizador animar_temp;

	void actualizarSprite();
	void sigFrame();

public:
	Bala();
	static bool inicializar();
	static void liberarMemoria();

	void actualizar();
	bool mover();
	void animar();
	void manejarEvento(SDL_Event& evento);

	void enColision(Colisionador *objeto);
	void Disparar(direccion_t direccion, int x, int y);
	void renderizar();
	void habilitarDestruccion(bool destruir);

	void enviarDestruirBloque(SDL_Point bloques[]);
	void enviarDestruirObjeto(int num_jugador, int tipo_objeto);
};

#endif // BALA_H_INCLUDED
