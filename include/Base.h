#ifndef BASE_H
#define BASE_H

#include "Objeto.h"
#include "utiles.h"

enum {
	BASE_NORMAL_CLIP,
	BASE_DESTRUIDA_CLIP,
	BASE_NUM_CLIPS
};

class Base: public Objeto {
private:
	static SDL_Texture *sprite;
	static SDL_Rect clips[BASE_NUM_CLIPS];
	bool esta_destruido;
	SDL_Rect clip;

public:
	static bool inicializar();
	static void terminar();
	Base(int x = 0, int y = 0);

	bool estaDestruido();
	void estaDestruido(bool destruido);
	void renderizar();
};

#endif // BASE_H