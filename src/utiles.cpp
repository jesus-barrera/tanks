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

SDL_Texture *renderizarTexto(string texto, SDL_Color color) {
	SDL_Surface *surface;
	SDL_Texture *textura = NULL;

	surface = TTF_RenderText_Blended(global_font, texto.c_str(), color);

	if (surface != NULL) {
		textura = SDL_CreateTextureFromSurface(renderer_principal, surface);

		SDL_FreeSurface(surface);
	}

	return textura;
}

void mostrarError(string mensaje) {
	cout << mensaje << ": " << SDL_GetError() << endl;
}