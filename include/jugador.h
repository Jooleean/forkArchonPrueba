#pragma once

#include <vector>

#include "animal.h"
#include "cabra.h"
#include "cerdo.h"
#include "estructuras.h"
#include "gallina.h"
#include "granjero.h"
#include "llama.h"
#include "oveja.h"

class Jugador 
{
	// esto nos lo dijo @psanse en la tutoria:
    std::vector<Animal*> animales_; 
    // para poder usar push_back y más mejoras que sirvan en el constructor cuando no haya
    // tantos parámetros de coordenadas en el constructor de Animal que ahora es un poco engorroso y no se entiende nada con tantos números mágicos.
	// antes era: Animal* animales_[Constantes::NUMERO_ANIMALES] = { nullptr }; con un array normal

    Animal* piezaSeleccionada_ = nullptr;
    Animal* animalEnCombate_ = nullptr;
    int equipo_;

    //template <typename T> void agregarAnimal(float& posX, float& posY, float& posZ, float dirX, float varExt2); Interesante para hacer la construcción más fácil 
    // una sola linea para crear todos los animales en un for al ser plantilla

public:

    Jugador(int equipo);
    ~Jugador();

    // Animales
    const std::vector<Animal*>& getAnimales() const { return animales_; }
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