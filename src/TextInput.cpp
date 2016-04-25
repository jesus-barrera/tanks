#include "../include/colores.h"
#include "../include/TextInput.h"

TextInput::TextInput(string prompt, int x, int y) {
	etiqueta = new Etiqueta(prompt, x, y, TEXTINPUT_TAM_FUENTE, COLOR_AZUL);
	input    = new Etiqueta("", x + etiqueta->obtenerAncho(), y, TEXTINPUT_TAM_FUENTE);
}

TextInput::~TextInput() {
	delete(etiqueta);
	delete(input);
}

void TextInput::actualizar() {
	etiqueta->renderizar();
	input->renderizar();
}

void TextInput::manejarEvento(SDL_Event &evento) {
	if (evento.type == SDL_KEYDOWN) {
		if (evento.key.keysym.sym == SDLK_BACKSPACE && !texto.empty()) {
			texto.pop_back();

			input->fijarTexto(texto);
		}
	} else if (evento.type == SDL_TEXTINPUT) {
		texto.append(evento.text.text);
		input->fijarTexto(texto);
	}
}

string TextInput::obtenerTexto() {
	return texto;
}

bool TextInput::vacio() {
	return texto.empty();
}