#ifndef _UTILES_INCLUDE_
#define _UTILES_INCLUDE_

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "globales.h"

using namespace std;

SDL_Texture *cargarTextura(char* nombre_archivo);
SDL_Texture *cargarTextura(char* nombre_archivo, Uint8 r, Uint8 g, Uint8 b);
SDL_Texture *renderizarTexto(string texto, SDL_Color);
void mostrarError(string mensaje);

#endif