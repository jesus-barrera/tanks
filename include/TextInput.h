#ifndef _TEXT_INPUT_H_INCLUDED_
#define _TEXT_INPUT_H_INCLUDED_

#include <string>
#include <SDL.h>
#include "Etiqueta.h"

class TextInput {
private:
    Etiqueta *etiqueta;
    Etiqueta *input;
    string texto;
    int max_len;

public:
    TextInput(string prompt, int x = 0, int y = 0, int max_len = -1);
    ~TextInput();

    void renderizar();
    void manejarEvento(SDL_Event &evento);

    string obtenerTexto();
    void fijarTexto(string texto);
    void limpiarTexto();
    bool vacio();
};

#endif
