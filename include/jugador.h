#pragma once
#include "animal.h"
#include "estructuras.h"

class Jugador 
{
    Animal* animales_[Constantes::numeroAnimales] = {};
    Animal* piezaSeleccionada_ = nullptr;
    Animal* animalEnCombate_ = nullptr;
    int equipo_;

public:

    Jugador(int equipo);
    ~Jugador();

    // Animales
    Animal* const* getAnimales() const { return animales_; }
    Animal* getAnimalEnCombate() { return animalEnCombate_; }
    void setAnimalEnCombate(Animal* animal) { animalEnCombate_ = animal; }

    // Pieza seleccionada
    void agarrarPieza(Animal* p) { piezaSeleccionada_ = p; }
    void soltarPieza() { piezaSeleccionada_ = nullptr; }
    bool tienePiezaAgarrada() const { return piezaSeleccionada_ != nullptr; }
    Animal* getPiezaSeleccionada() const { return piezaSeleccionada_; }

    // Identidad
    int getEquipo() const { return equipo_; }
};