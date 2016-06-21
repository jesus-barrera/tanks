# Tanks
Juego 2D de tanques desarrollado en SDL.

### Características
* Multijugador en red
* Editor de mapas
* Compartir mapas creados por el usuario
* Dos modos de juego: 
    - Por vidas: el primero en quedarse sin vidas pierde
    - Por base: el primero en destruir la base enemiga gana

### Requerimientos
Para poder compilar el proyecto necesitas las siguientes versiones de SDL
* SDL 2.0
* SDL Image 2.0.1
* SDL ttf 2.0.0

### Compilar
En Linux:
```
g++ main.cpp src/* -std=c++11 -lSDL2 -lSDL2_image -lSDL2_ttf -I/usr/include/SDL2 -o tanques
```