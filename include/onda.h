#pragma once
#include "ataque.h"

class Onda : public Ataque {

    float expansion_ = 50.0f;  // píxeles por segundo que crece
    float tamanoInicial_;

public:
    Onda(int dano, float alcance, float recarga, const char* sprite, float tamano, float duracion)
        : Ataque(dano, alcance, recarga, sprite, tamano, duracion),
        tamanoInicial_(tamano) {}

    void activar(float x, float y, float dirX, float dirY) override {
        Ataque::activar(x, y, dirX, dirY);
        tamano_ = tamanoInicial_;  // resetear tamaño al activar
    }

    void mover(float dt) override {
        if (!activo_) return;

        // La onda crece con el tiempo
        tamano_ += expansion_ * (dt / 1000.0f);
        tiempoActivo_ += dt / 1000.0f;

        if (tiempoActivo_ >= duracion_visual_)
            desactivar();
    }
};