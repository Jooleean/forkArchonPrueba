#pragma once
#include "animal.h"

class Gallina : public Animal 
{
public:

	Gallina(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 5;
		max_casillas_movidas_ = 2;
		nFrames = 8;
		ataque_ = 5;
		especie_ = GALLINA;
	}
};