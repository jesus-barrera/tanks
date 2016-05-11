#ifndef MUSICA_H_INCLUDED
#define MUSICA_H_INCLUDED

#include <SDL_mixer.h>
#include <SDL.h>

#include <iostream>
#include <cstring>

using namespace std;

enum{
    MusicaFondoMenu,
    MusicaFondoCrearPartida,
    MusicaFondoBuscarPartida,
    MusicaFondoCrearMapa,
    MusicaFondoJugar,
    TotalFondos
};

enum{
    Snd_Disparo,
    Snd_ColisionMetal,
    Snd_ColisionBloque,
    Snd_Explosion,
    Snd_Click_boton,
    Snd_Movimiento_tanque,
    TotalSonidos
};

int cambiarMusicaFondo(int numMusica);
void ReproducirMusicaFondo();
void DetenerCanal(int canal);
void ReproducirSonido(int numSonido, int Volumen, int Canal, int repeticiones);
void inicializarMusica();

#endif // MUSICA_H_INCLUDED
