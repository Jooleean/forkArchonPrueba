#pragma once
#include "animal.h"

class Cabra: public Animal 
{
public:

	Cabra(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = 20;
		max_casillas_movidas_ = 4;
		nFrames = 5;
		especie_ = CABRA;
	}

	//void actualizar(float dt) override;
};