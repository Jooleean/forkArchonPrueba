#pragma once
#include "animal.h"
#include "disparo.h"
#include "estructuras.h"


class Llama : public Animal
{
public:

	Llama(Casilla casillaInicial, int equipo) : Animal(casillaInicial, equipo)
	{
		vida_ = ParametrosAnimales::VIDA_LLAMA;
		vida_base_ = ParametrosAnimales::VIDA_LLAMA;
		max_casillas_movidas_ = 6;
		nFrames = 5;
		especie_ = LLAMA;
		ataque_ = new Disparo(ParametrosAnimales::DANO_LLAMA, ParametrosAnimales::DURACION_LLAMA,
			ParametrosAnimales::RECARGA_LLAMA, "../assets/Sprites/menu/selector.png", 15.0f, 0.35f);
	}
	const char* getTipoAtaque() const override { return "Disparo"; }
	//void actualizar(float dt) override;
};