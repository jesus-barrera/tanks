#ifndef COLISIONADOR_H
#define COLISIONADOR_H

#include <SDL.h>
#include <vector>
#include <string>

using namespace std;

class Colisionador {
protected: 
	vector<Colisionador *> colisionadores;
	SDL_Rect *area;
	string etiqueta;

public:

	Colisionador();

	void fijarAreaColision(SDL_Rect *area);
	SDL_Rect *obtenerAreaColision();

	void agregarColisionador(Colisionador *obj);
	SDL_Rect *comprobarColision(SDL_Rect *rect, SDL_Rect *colision_area);
	bool tieneEtiqueta(string etiqueta);
	
	virtual void enColision(Colisionador *obj) = 0;
}; 

#endif // COLISIONADOR_H