#pragma once
#include "animal.h"
#include "onda.h"
#include "estructuras.h"


class Cerdo : public Animal
{
public:

	Cerdo(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = ParametrosAnimales::VIDA_CERDO;
		vida_base_ = ParametrosAnimales::VIDA_CERDO;
		max_casillas_movidas_ = 3;
		nFrames = 5;
		especie_ = CERDO;
		ataque_ = new Onda(ParametrosAnimales::DANO_CERDO, ParametrosAnimales::DURACION_CERDO,
			ParametrosAnimales::RECARGA_CERDO,"../assets/Sprites/menu/selector.png", 20.0f, 0.6f);
	}
	const char* getTipoAtaque() const override { return "Onda"; }
	//void actualizar(float dt) override;
};