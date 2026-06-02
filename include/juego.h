#pragma once

#include <iostream>
#include <stdlib.h>

#include "menu.h"
#include "animal.h"
#include "arena.h"
#include "controles.h"
#include "creditos.h"
#include "estructuras.h"
#include "jugador.h"
#include "opciones.h"
#include "renderizador.h" 
#include "RenderizadorAudio.h"
#include "tablero.h"
#include "transicion.h" 

class Juego 
{   
    Transicion transicion_;
    Menu* menu_;
    Tablero* tablero_;
    Arena* arena_;
    renderizadorAudio* audio_;
    Renderizador* renderizador_;
    Jugador* jugadores_[2];
    Creditos* creditos_;
    Controles* controles_;
    Opciones* opciones_;
    Ganador* ganador_;

public:

    enum Estado { MENU, TABLERO, BATALLA, OPCIONES, CONTROLES, CREDITOS, GANADOR};
    Estado estado_actual;
    Estado proximo_estado;

    Juego(); 
    ~Juego();

    // game loop dividido en dos fases desacopladas
    void actualizarLogica(float dt); // se llama desde OnTimer
    void renderizarGraficos(); // se llama desde OnDraw

    // coordinador de inputs de teclado
    void procesarTeclaPresionada(unsigned char key);
    void procesarTeclaLevantada(unsigned char key);
    void procesarTeclaEspecialPresionada(int key);
    void procesarTeclaEspecialLevantada(int key);
};