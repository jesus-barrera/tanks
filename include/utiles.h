#ifndef _UTILES_INCLUDE_
#define _UTILES_INCLUDE_

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "juego.h"

using namespace std;

SDL_Texture *cargarTextura(char* nombre_archivo);
void mostrarError(string mensaje);

#endif