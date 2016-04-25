#include <map>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "../include/globales.h"
#include "../include/utiles.h"

map<int, TTF_Font *> fuentes;

SDL_Texture *cargarTextura(string nombre_archivo) {
	SDL_Surface *surface;
	SDL_Texture *textura = NULL;

	surface = IMG_Load(nombre_archivo.c_str());

	if (surface != NULL) {
		textura = SDL_CreateTextureFromSurface(renderer_principal, surface);

		SDL_FreeSurface(surface);
	}

	return textura;
}

SDL_Texture *cargarTextura(string nombre_archivo, Uint8 r, Uint8 g, Uint8 b) {
		SDL_Surface *surface;
	SDL_Texture *textura = NULL;

	surface = IMG_Load(nombre_archivo.c_str());

	if (surface != NULL) {
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b));
		
		textura = SDL_CreateTextureFromSurface(renderer_principal, surface);

		SDL_FreeSurface(surface);
	}

	return textura;
}

SDL_Texture *renderizarTexto(string texto, SDL_Color color, int tam_fuente) {
	SDL_Surface *surface;
	SDL_Texture *textura = NULL;
	TTF_Font *font;

	if (fuentes.count(tam_fuente)) {
		font = fuentes[tam_fuente];
	} else {
		font = TTF_OpenFont("media/fonts/Roboto-Regular.ttf", tam_fuente);

		if (font == NULL) {
			return NULL;
		} else  {
			fuentes[tam_fuente] = font;
		}
	}
	
	surface = TTF_RenderText_Blended(font, texto.c_str(), color);

	if (surface != NULL) {
		textura = SDL_CreateTextureFromSurface(renderer_principal, surface);

		SDL_FreeSurface(surface);
	}

	return textura;
}

void cerrarFuentes() {
	map<int, TTF_Font *>::iterator it;

	for (it = fuentes.begin(); it != fuentes.end(); it++) {
		TTF_CloseFont(it->second);
	}

	fuentes.clear();
}

void mostrarError(string mensaje) {
	cout << mensaje << ": " << SDL_GetError() << endl;
}