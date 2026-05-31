#pragma once
#include "animal.h"
#include "golpe.h"

class Granjero : public Animal
{
public:

	Granjero(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = ParametrosAnimales::VIDA_GRANJERO;
		vida_base_ = ParametrosAnimales::VIDA_GRANJERO;
		max_casillas_movidas_ = 12;
		nFrames = 5;
		especie_ = GRANJERO;
		ataque_ = new Golpe(ParametrosAnimales::DANO_GRANJERO, ParametrosAnimales::DURACION_GRANJERO,
			ParametrosAnimales::RECARGA_GRANJERO, "../assets/Sprites/menu/selector.png", 16.0f, 0.5f);
	}
	const char* getTipoAtaque() const override { return "Golpe"; }
	//void actualizar(float dt) override;
};