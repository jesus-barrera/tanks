#include "../include/utiles.h";

SDL_Texture *cargarTextura(char *nombre_archivo) {
		SDL_Surface *surface;
		SDL_Texture *textura = NULL;

		surface = IMG_Load(nombre_archivo);

		if (surface != NULL) {
			textura = SDL_CreateTextureFromSurface(renderer_principal, surface);

			SDL_FreeSurface(surface);
		}

		return textura;
}