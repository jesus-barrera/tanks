#ifndef _TEXT_INPUT_H_INCLUDED_
#define _TEXT_INPUT_H_INCLUDED_

#include <string>
#include <SDL.h>
#include "Etiqueta.h"

#define TEXTINPUT_TAM_FUENTE 40

class TextInput {
private:
	Etiqueta *etiqueta;
	Etiqueta *input;
	string texto;

public:
	TextInput(string prompt, int x = 0, int y = 0);
	~TextInput();

	void actualizar();
	void manejarEvento(SDL_Event &evento);
	
	string obtenerTexto();
	bool vacio();
};

#endif