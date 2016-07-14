# Tanks
Juego 2D de tanques basado en el clásico juego de NES *Battle City*.

### Características
* Multijugador (1 vs 1) en red
* Editor de mapas: Crea, guarda, edita y juega en tus propios mapas
* Dos modos de juego: 
    - Por vidas: Destruye a tu adversario hasta que se quede sin vidas
    - Por base: Destruye la base de tu adversario mientras defiendes la tuya

### Requerimientos
Para poder compilar el proyecto necesitas las siguientes versiones de SDL
* SDL 2.0.3
* SDL image 2.0.1
* SDL ttf 2.0.0
* SDL mixer 

### Compilar
En Linux:
```
g++ main.cpp src/* -std=c++11 -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -I/usr/include/SDL2 -o tanques
```