#pragma once
#include "animal.h"
#include "embestida.h"
#include "estructuras.h"

class Cabra : public Animal
{
public:

	Cabra(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = ParametrosAnimales::VIDA_CABRA;
		vida_base_ = ParametrosAnimales::VIDA_CABRA;
		max_casillas_movidas_ = 4;
		nFrames = 5;
		especie_ = CABRA;
		ataque_ = new Embestida(ParametrosAnimales::DANO_CABRA, ParametrosAnimales::DURACION_CABRA,
			ParametrosAnimales::RECARGA_CABRA,"../assets/Sprites/creditos/botella.png", 20.0f);
	} 

};