#ifndef _ETIQUETA_H_INCLUDED_
#define _ETIQUETA_H_INCLUDED_

#include <SDL.h>
#include <string>

using namespace std;

class Etiqueta {
protected:
	string texto;
	SDL_Texture *textura;
	SDL_Color color;
	SDL_Rect rect;
	int tam_fuente;

	void actualizarTextura(bool redimensionar);

public:
	Etiqueta(string texto, int x = 0, int y = 0, int tam_fuente = 25, SDL_Color color = {0, 0, 0});
	~Etiqueta();

	void fijarTexto(string texto);
	void fijarTamFuente(int tam);
	void fijarColor(SDL_Color color);
	void fijarPosicion(int x, int y);

	string obtenerTexto();

	void renderizar();
};

#endif