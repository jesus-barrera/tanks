#include "../include/Editor.h"

int Editor::bloque_seleccionado = BLOQUE_BRICK;

void Editor::manejarEvento(SDL_Event &evento) {
	if (evento.type == SDL_MOUSEBUTTONDOWN) {
		int boton = evento.button.button;
		SDL_Point bloque_pos = Escenario::puntoAPosicionMapa(evento.button.x, evento.button.y);

		if (boton == SDL_BUTTON_LEFT) {
			Escenario::insertarBloque(bloque_pos, bloque_seleccionado);
		} else if (boton == SDL_BUTTON_RIGHT) {
			Escenario::insertarBloque(bloque_pos, NO_BLOQUE);
		}
	} else if (evento.type == SDL_KEYDOWN && evento.key.repeat == 0) {
		switch (evento.key.keysym.sym) {
			case SDLK_1:
				bloque_seleccionado = BLOQUE_BRICK;
				break;
			case SDLK_2:
				bloque_seleccionado = BLOQUE_METAL;
				break;
			case SDLK_3:
				bloque_seleccionado = BLOQUE_AGUA_1;
				break;
			case SDLK_4:
				bloque_seleccionado = BLOQUE_ARBUSTO;
				break;
			case SDLK_r:
				Escenario::limpiarMapa();
				break;
			case SDLK_s:
				Escenario::guardarMapa();
				break;
			case SDLK_l:
				Escenario::cargarMapa();
				break;
			default: 
				;
		}
	}
}

void Editor::renderizar() {
	SDL_Rect bloque_rect;
	SDL_Texture *bloque;

	bloque = Escenario::obtenerTexturaBloque(bloque_seleccionado);
	
	bloque_rect.w = 50;
	bloque_rect.h = 50;

	bloque_rect.x = vista_estatus.w / 2 - bloque_rect.w / 2;
	bloque_rect.y = vista_estatus.h / 2 - bloque_rect.h / 2;

	SDL_RenderSetViewport(renderer_principal, &vista_juego);
	Escenario::renderizar();
	
	SDL_RenderSetViewport(renderer_principal, &vista_estatus);
	SDL_RenderCopy(renderer_principal, bloque, NULL, &bloque_rect);
}