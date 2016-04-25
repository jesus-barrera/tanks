#include "../include/colores.h"
#include "../include/TextInput.h"

TextInput::TextInput(string prompt, int x, int y, int max) {
	etiqueta = new Etiqueta(prompt, x, y, DEFAULT_FONT_SIZE, COLOR_AZUL);
	input    = new Etiqueta("", x + etiqueta->obtenerAncho(), y);
	max_len  = max;
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
	} else if (evento.type == SDL_TEXTINPUT && (max_len < 0 || texto.size() < max_len)) {
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