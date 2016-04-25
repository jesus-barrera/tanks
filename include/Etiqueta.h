#ifndef _ETIQUETA_H_INCLUDED_
#define _ETIQUETA_H_INCLUDED_

#include <SDL.h>
#include <string>
#include "colores.h"

#define DEFAULT_FONT_SIZE 20

using namespace std;

class Etiqueta {
protected:
	string texto;
	SDL_Texture *textura;
	SDL_Color color;
	SDL_Rect rect;
	int tam_fuente;

	void actualizarTextura();

public:
	Etiqueta(string texto, int x = 0, int y = 0, int tam_fuente = DEFAULT_FONT_SIZE, SDL_Color color = COLOR_BLANCO);
	~Etiqueta();

	void fijarTexto(string texto);
	void fijarTamFuente(int tam);
	void fijarColor(SDL_Color color);
	void fijarPosicion(int x, int y);

	int obtenerAncho();
	int obtenerAltura();
	string obtenerTexto();

	void renderizar();
};

#endif