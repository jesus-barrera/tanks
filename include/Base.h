#ifndef BASE_H
#define BASE_H

#include "Objeto.h"
#include "Colisionador.h"
#include "utiles.h"

#define BASE_ETIQUETA "base"

enum {
	BASE_NORMAL_CLIP,
	BASE_DESTRUIDA_CLIP,
	BASE_NUM_CLIPS
};

class Base: public Colisionador, public Objeto {
private:
	static SDL_Texture *sprite;
	static SDL_Rect clips[BASE_NUM_CLIPS];

	bool esta_destruido;
	SDL_Rect clip;

public:
	static bool inicializar();
	static void terminar();

	Base();

	bool estaDestruido();
	void estaDestruido(bool destruido);
	void renderizar();

	void enColision(Colisionador *objeto);
};

#endif // BASE_H