#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
#include "../include/Musica.h"

using namespace std;

#define RUTAFONDOS "media/Sound/Fondos/"
#define RUTASONIDOS "media/Sound/Sonidos/"

Mix_Music *Mus_fondo;
Mix_Chunk *sonidos[TotalSonidos];

string NombreMusicafondo[TotalFondos] = {
    RUTAFONDOS "The Legend of Zelda - NES - Overworld.mp3",
    RUTAFONDOS "",
    RUTAFONDOS "",
    RUTAFONDOS "Theme for Harold var 3.mp3",
    RUTAFONDOS "Pinball Spring.mp3"
};

string NombresSonidos[TotalSonidos] = {
    RUTASONIDOS "Disparo 1.wav",
    RUTASONIDOS "Choque - Metal-.wav",
    RUTASONIDOS "Choque Bloque .wav",
    RUTASONIDOS "Explosion.wav",
    RUTASONIDOS "Seleccion-boton.wav",
    RUTASONIDOS "Movimiento-Tanque.wav"
};

int cambiarMusicaFondo(int numMusica){
    /**
    *Poner codigo para que dependiendo del numero de la muscia
    *se ponga dicho string que contiene la direccion
    */
    Mus_fondo = Mix_LoadMUS(NombreMusicafondo[numMusica].c_str());
    if(Mus_fondo == NULL){
        cout<<Mix_GetError()<<endl;
        cout<<"Error al cargar la musica... D:"<<endl;
        return -1;
    }else{
        return 0;
        cout<<"Aun funciona la musica"<<endl;
    }
}

void ReproducirMusicaFondo(){
    Mix_VolumeMusic(30);
    if(Mix_PlayMusic(Mus_fondo, -1)==-1){
        cout<<"El error esta aqui"<<endl;
    }
}

void ReproducirSonido(int numSonido, int volumen, int canal, int repeticiones){
    Mix_VolumeChunk(sonidos[numSonido], volumen);
    Mix_PlayChannel(canal, sonidos[numSonido], repeticiones);
}

void DetenerCanal(int canal){
   Mix_Pause(canal);
}

void inicializarMusica(){
    int i=0;

    if(SDL_InitSubSystem (SDL_INIT_AUDIO)<0){
        cout<<"Error en el sonido"<<endl;
    }else{
        cout<<"Se abrio el canal de audio"<<endl;
        Mix_OpenAudio(44100, AUDIO_S16, 2, 4096);
    }

    for(i=0; i<TotalSonidos; i++){
        sonidos[i] = Mix_LoadWAV(NombresSonidos[i].c_str());
    }

}
