#ifndef _UTILES_INCLUDE_
#define _UTILES_INCLUDE_

#include <iostream>
#include <SDL.h>

using namespace std;

SDL_Texture *cargarTextura(string nombre_archivo);
SDL_Texture *cargarTextura(string nombre_archivo, Uint8 r, Uint8 g, Uint8 b);
SDL_Texture *renderizarTexto(string texto, SDL_Color, int tam_fuente);
void renderizarCapaGris();
void cerrarFuentes();
void mostrarError(string mensaje);

#endif