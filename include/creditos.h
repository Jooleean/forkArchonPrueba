#pragma once
#include "renderizador.h"

class BotellaCreditos
{
	float timer_ = 0;
	float msStep_ = 100;
	int nFrames_ = 8;
	bool loop_ = false;

public:

	float posx_ = 364;
	const float posy_ = 25;
	int frameActualX_ = 0;
	int frameActualY_ = 1;

	void animar(float dt);
};

class GallinaCreditos 
{
	float timer_ = 0;
	float msStep_ = 100;
	int nFrames_ = 8;

public:

	const float posx_ = 40;
	const float posy_ = 37;
	int frameActualX_ = 0;
	int frameActualY_ = 1;

	void animar(float dt);
};

class Creditos 
{
private:

	BotellaCreditos botella_;
	GallinaCreditos gallina_;
	float posx_;
	float posy_;
	const float posFinal_ = -246;
	float capaz_;
	bool fin_;

public:

	void reset();
	Creditos() { reset(); }
	bool getFinalizado() const { return fin_; }

	void actualizar(float dt);
	void dibujar(Renderizador* motor);
};