#ifndef BASE_H
#define BASE_H

#include "globales.h"
#include "Objeto.h"
#include "Colisionador.h"
#include "utiles.h"

#define BASE_ETIQUETA "base"

enum {
	BASE_NORMAL_CLIP,
	BASE_DESTRUIDA_CLIP,
	BASE_NUM_CLIPS
};

enum {
	BASE_TIPO_ROJO,
	BASE_TIPO_AZUL
};

class Base: public Colisionador, public Objeto {
private:
	static SDL_Texture *sprite;
	static SDL_Rect clips[BASE_NUM_CLIPS];

	bool esta_destruido;
	SDL_Rect clip;

	int tipo;

public:
	static bool inicializar();
	static void liberarMemoria();

	Base(int tipo = BASE_TIPO_ROJO);

	bool estaDestruido();
	void estaDestruido(bool destruido);
	void renderizar();

	void enColision(Colisionador *objeto);
};

#endif // BASE_H